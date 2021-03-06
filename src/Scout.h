#pragma once




/**********************
* C S Cowden      2 February 2019
* Scout (a sub-class of ArmyManager).
***********************/

#include <string>


#include "Common.h"
#include "bot.h"
#include "ArmyManager.h"

#include "weaver.h"
#include "HUD.h"
#include "Logger.h"

#include "UnitManager.h"
#include "UnitProxy.h"

namespace Turk {

	class Scout : public ArmyManager {
	public:

		/**
		* Default constructor
		*/
		inline Scout() : ArmyManager("Scout") {

			// get name and increment agent count
			m_instance = m_nScouts++;
			m_name = "Scout_" + std::to_string(m_instance);

			// register agent with unit manager
			umanity.register_agent(this);

			std::stringstream msg;
			msg << "Creating agent ";
			msg << name() << " of type: " << type() << " at: 0x" << std::hex << (int)this;
			Turk::Logger::instance()->log(name().c_str(), msg.str().c_str());

			// get a lane in the HUD
			hud_lane_ = HUD::Instance().getLane(this);

			// this agent is alive, but no units
			isAlive_ = true;
			units_loaded_ = false;

			// start active state at null
			active_state_ = active;
			filled_locations_ = false;

			// fill the start location queue
			fill_location_queue();
		}


		/**
		* constructor with a given name
		*/
		inline Scout(const char * name) : ArmyManager(name) { }

		/**
		* Delete this instance
		*/
		virtual inline ~Scout() { 

			// delete squads
			for (unsigned i = 0; i != squads_.nheld(); i++) {
				delete squads_[i];
			}

			// unregister this agent
			umanity.unregister_agent(this);
		}

		/**
		* Execute a given command encoded as an integer
		*/
		virtual int execute(int command, const bot_args & args) { return 0; }

		/**
		* process queue - to be called every frame for actions needed to take.
		*/
		inline virtual void process() { 

			updateHUD();

			const unsigned ns = squads_.nheld();

			// unwind when all units are destroyed
			unsigned nunits = 0U;
			for (unsigned i = 0; i != ns; i++)
				nunits += squads_[i]->size();

			if (isAlive_ && units_loaded_ && nunits == 0) {
				isAlive_ = false;
				active_state_ = null;
			}


			// process components
			for (unsigned i = 0; i != ns; i++)
				squads_[i]->process();

			// update the location
			loc_ = BWAPI::Positions::Origin;
			double s = 0U;

			for (unsigned i = 0; i != ns; i++) {
				loc_ += squads_[i]->location()*squads_[i]->size();
				s += squads_[i]->size();
			}
			if (s) loc_ /= s;


			// if state is active and there are units, go scout
			if (active_state_ == active && squads_.size() > 0) {

				// check for enemies,
				// if found, evade
				const BWAPI::Unitset & units = BWAPI::Broodwar->getUnitsInRadius(BWAPI::Position(squads_[0]->location()), 100, BWAPI::Filter::IsEnemy);
				//const BWAPI::Unitset & units = BWAPI::Broodwar->getUnitsInRadius(BWAPI::Position(pos_), 500, BWAPI::Filter::IsEnemy);
				if (units.size() > 0) {
					active_state_ = evade;
				}


				// if the active queue is emtpy
				// pop one frome the location queue and add it to the active queue
				// and send the squad to the location
				if (active_queue_.nqueued() == 0) {
					BWAPI::TilePosition pos = base_queue_.pop();
					while (pos.getDistance(BWAPI::TilePosition(artie.get_start_pos())) < 100)
						pos = base_queue_.pop();

					active_queue_.push(pos);

					Turk::squad_args args;
					args.command |= SQUAD_MOVE;
					args.location = Turk::location(pos);
					squads_[0]->execute(args);

				}
				// if the location has been reached,
				// remove the active location from the queue
				else if (active_queue_.nqueued() > 0 && active_queue_[0].getDistance(BWAPI::TilePosition(squads_[0]->location())) < 5) {
					active_queue_.pop();

					// leave active state if  no more items in queue go to inactive state
					if (base_queue_.nqueued() == 0)
						active_state_ = complete;
				}
			}

			else if (active_state_ == active && squads_.size() == 0) {
				// assume total loss of units.
				active_state_ = complete;
			}


		}

		/**
		* Add units to this agent's control
		*/
		inline virtual void addUnits(const std::vector<UnitProxy> & units) {

			// keep it simple and add units to a squad
			if (squads_.size() == 0) {
				Turk::Squad * sq = new Turk::Squad();
				squads_.push(sq);
			}

			//squads_[0]->addUnits(units);
			umanity.transfer(this, squads_[0], units);

			// the agent is definitely alive if it gets reinforcements
			isAlive_ = true;
			units_loaded_ = true;

		}

		inline virtual UnitProxy removeUnit(const BWAPI::UnitType & ut) { return UnitProxy(); }

		inline virtual void updateUnits() { 
			// transfer all units to squad
			if (umanity.getUnits(this).size() > 0 ) {
				if (squads_.size() == 0) {
					Turk::Squad *sq = new Turk::Squad();
					squads_.push(sq);
				}
				umanity.transfer(this, squads_[0], umanity.getUnits(this));
				isAlive_ = true;
				units_loaded_ = true;
			}
		}


		inline bool isAlive() const { return isAlive_; }

	protected:

		void updateHUD();

	private:

		inline void fill_location_queue() {

			// get list of start positions
			BWAPI::TilePosition::list positions = BWAPI::Broodwar->getStartLocations();

			const unsigned n = positions.size();
			for (unsigned i = 0; i != n; i++) {
				base_queue_.push(positions[i]);
			}

			filled_locations_ = true;
		}

		// track the number of scout agents
		static unsigned m_nScouts;
		unsigned m_instance;

		Turk::vvec<Squad *> squads_;

		// flag if all units have been lost
		bool isAlive_;

		// flag to indicate units have been loaded
		bool units_loaded_;

		// keep track of destination queues
		vqueue<BWAPI::TilePosition> base_queue_;
		vqueue<BWAPI::TilePosition> active_queue_;

		bool filled_locations_;

		// state 
		//  - scout (search starting locations)
		//  - evade 
		//  - null
		enum TheState {
			null,
			active,
			evade,
			complete
		} active_state_;
		

		//std::string m_name;
	};
}