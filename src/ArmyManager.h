#pragma once

/*********************************
* C S Cowden     19 June 2018
* Army Manager - manages combat units
* or other army manager units.
**********************************/

#include <string>

#include "Common.h"
#include "bot.h"

#include "weaver.h"
#include "HUD.h"
#include "Logger.h"

#include "UnitManager.h"
#include "UnitProxy.h"
#include "Squad.h"

namespace Turk {

/**
* Argument struct for parsing commands to the agent.
*/
struct army_args : bot_args {
};

class ArmyManager : public bot {
public:
		/**
		* Default constructor
		*/
		inline ArmyManager():bot("ArmyManager") {

			scout_ = NULL;

			// get name and increment agent count
			m_instance = m_nArmies++;
			m_name = "ArmyManager_" + std::to_string(m_instance);

			// register agent with unit manager
			umanity.register_agent(this);

			std::stringstream msg;
			msg << "Creating agent ";
			msg << name() << " of type: " << type() << " at: 0x" << std::hex << (int)this;
			Turk::Logger::instance()->log(name().c_str(), msg.str().c_str());

			// get a lane in the HUD
			hud_lane_ = HUD::Instance().getLane(this);
		}

		/**
		* constructor with a given name
		*/
		inline ArmyManager(const char * name) : bot(name) { }

		
		/**
		* Delete this instance
		*/
		inline ~ArmyManager() { }

		/**
		* Execute a given command encoded as an integer
		*/
		virtual int execute(int command, const bot_args & args);

		/**
		* Return the bot type
		*/
		//virtual const std::string & type() const { return "0"; }

		/**
		* Return the location
		*/
		virtual const Turk::location & location() const { return loc_; }

		/**
		* Return the status
		*/
		virtual const Turk::status status() const { return status_; }

		/**
		* Load a model
		*/
		virtual void loadModel(const model_args & args) { }

		/**
		* Dump the model
		*/
		virtual void dumpModel(const model_args & args) {}

		/**
		* Get the model name
		*/
		virtual inline const std::string & name() const { return m_name; }


  /**
  * process queue - to be called every frame for actions needed to take.
  */
		inline virtual void process() { 

			// process components
			const unsigned ns = squads_.nheld();
			for (unsigned i=0; i != ns; i++)
				squads_[i]->process();

			const unsigned na = armies_.nheld();
			for (unsigned i=0; i != na; i++)
				armies_[i]->process();

			if (scout_) scout_->process();

			// update location
			loc_ = BWAPI::Positions::Origin;
			double s = 0U;

			
			for (unsigned i = 0; i != ns; i++) {
				loc_ += squads_[i]->location()*squads_[i]->size();
				s += squads_[i]->size();
			}
			
			for (unsigned i = 0; i != na; i++) {
				loc_ += armies_[i]->location()*armies_[i]->size();
				s += armies_[i]->size();
			}
			if (s) loc_ /= s;
			

			// send units to the base choke point
			// find the nearest choke point
			const BWAPI::WalkPosition & base_loc = artie.get_start_pos();
			unsigned nc = 0;
			double dist = DBL_MAX;
			const unsigned N = artie.getARTIE().get_chokes().size();
			for (unsigned i = 1; i != N; i++) {
				const Turk::region & reg = artie.getARTIE()[i];
				if (reg.depth() < 15) {
					BWAPI::WalkPosition pos(reg.position().x, reg.position().y);
					double d = base_loc.getDistance(pos);
					if (d < dist) {
						dist = d;
						nc = i;
					}
				}
			}

			// keep units near choke point
			const Turk::region & choke = artie.getARTIE()[nc];
			const BWAPI::Position choke_pos(BWAPI::WalkPosition(choke.position().x, choke.position().y));
			// cycle over squads, if dist > X move to location
			for (unsigned i = 0; i != ns; i++) {
				double dist = choke_pos.getDistance(squads_[i]->location());

				if (dist > 50) {
					// issue move command
					Turk::squad_args args;
					args.command |= SQUAD_MOVE;
					args.location = choke_pos;

					squads_[i]->execute(args);
				}
			}

		}

		inline virtual void addUnits(const std::vector<UnitProxy> & units) { 

			// keep it simple and add units to a squad
			if (squads_.size() == 0) {
				Turk::Squad * sq = new Turk::Squad();
				squads_.push(sq);
			}

			//squads_[0]->addUnits(units);
			umanity.transfer(this, squads_[0], units);

		}

		inline virtual UnitProxy removeUnit(const BWAPI::UnitType & ut) { return UnitProxy(); }

		inline virtual void updateUnits() { }

		/**
		*  return the size of the army as the number of units 
		* all squads and sub-armies.
		*/
		inline virtual unsigned size() const {
			unsigned s = 0U;

			// sum over squads
			const unsigned ns = squads_.nheld();
			for (unsigned i=0; i != ns; i++)
			  s += squads_[i]->size();

			// sum over armies
			const unsigned na = armies_.nheld();
			for (unsigned i=0; i != na; i++)
				s += armies_[i]->size();

			return s;
		}

protected:

	Turk::location loc_;
	Turk::status status_;

	// bot name
	std::string m_name;

	// HUD lane
	int hud_lane_;


private:

	void initiate_scout();

		// track the number of army agents
		static unsigned m_nArmies;
		unsigned m_instance;

	    Turk::vvec<Squad *> squads_;
		Turk::vvec<ArmyManager *> armies_;
		
		ArmyManager * scout_;

};

}  // end Turk namespace
