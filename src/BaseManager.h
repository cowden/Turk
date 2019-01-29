#ifndef BASEMANAGER_H
#define BASEMANAGER_H

/********************************
* C S Cowden   19 June 2018
* Base class for base management.
* Base management encompasses resource gathering
* unit production, tech, and base expansion
*********************************/

#include <string>

#include "Common.h"
#include "bot.h"

#include "weaver.h"
#include "HUD.h"
#include "Logger.h"

#include "UnitManager.h"
#include "UnitProxy.h"



namespace Turk {


/**
* argument struct for parsing commands to the agent.
*/
struct base_args : bot_args {

  /**
  * collection of desired units and counts.
  */
  std::vector<std::pair<TUnit,unsigned> > unit_count_;

};


/**
* build order struct - encapsulate an item in a build order
* * building/research
* * supply level
*/
struct build_order_struct {
	unsigned supply;
	unsigned supply_base;
	TUnit unit;

	build_order_struct(){ }

	build_order_struct(unsigned s, unsigned b, const TUnit & u):
		supply(s),supply_base(b),unit(u) 
	{}
};


/**
* build prep struct - encapsulate the UnitType of a building
* the assigned worker and the build location
*/
struct build_prep_struct {
	BWAPI::UnitType building_type;
	BWAPI::Unit worker;
	BWAPI::TilePosition build_tile;

	build_prep_struct() { }

	build_prep_struct(BWAPI::UnitType bt, BWAPI::Unit w, BWAPI::TilePosition pos) :
		building_type(bt), worker(w), build_tile(pos)
	{ }
};


/*
*  build construction (ongoing) struct - encapsulate the Unit of the building,
* the assigned worker (if such exists)
*/
struct build_construct_struct {
	BWAPI::Unit building;
	BWAPI::Unit worker;

	build_construct_struct() { }

	build_construct_struct(BWAPI::Unit b, BWAPI::Unit w) :
		building(b), worker(w)
	{ }
};

/**
* Agent which implements models to operate a single base.
*/
class BaseManager : public bot {
public:
	/**
	* Default constructor
	*/
	inline BaseManager() :bot("BaseManager") {

		// get name and increment agent count
		m_instance = m_nBases++;
		m_name = "BaseManager_" + std::to_string(m_instance);

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
	* Delete this instance
	*/
	inline ~BaseManager() { }

	/**
	* Execute a given command encoded as an integer
	*/
	virtual int execute(int command, const bot_args & args) { return 0; }

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
	void loadModel(const model_args & args);

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
		// update the HUD
		updateHUD();

		// check for IRQ commands

		// get more workers if necessary
		if (BWAPI::Broodwar->self()->minerals() >= 100
			&& depot_->getTrainingQueue().size() < 5
			&& workers_.size() + depot_->getTrainingQueue().size() < 10
			) {
			depot_->train(race_.getWorker());
		}

		// check for idle workers
		//BWAPI::Unit tunit = depot_->getBuildUnit();
		//if (tunit) workers_.push_back(tunit);
		bool construction = false;
		for (auto up : workers_) {
			BWAPI::Unit & u = up.getUnit();
			if (u->isIdle()) {
				u->gather(u->getClosestUnit(BWAPI::Filter::IsMineralField));
			}
			if (u->isConstructing()) {
				construction = true;
				BWAPI::Broodwar->drawCircleMap(u->getTargetPosition(), 25, BWAPI::Colors::Purple);
			}
		}

		// check the ongoing construction queue
		if (build_construct_queue_.nheld() > 0) {
		}

		// check the prep queue
		if (build_prep_queue_.nheld() > 0) {
			const unsigned nq = build_prep_queue_.nheld();
			for (unsigned i = 0; i != nq; i++) {
				build_prep_struct & bs = build_prep_queue_[i];
				if (bs.worker->isConstructing()) continue;

				// check resources
				if (bs.building_type.mineralPrice() <= BWAPI::Broodwar->self()->minerals()
					&& bs.building_type.gasPrice() <= BWAPI::Broodwar->self()->gas()) {
					// let's allow for a new location
					build_prep_struct bs_new = building(bs.building_type);
					bs.worker = bs_new.worker;
					bs.build_tile = bs_new.build_tile;
				}
				// check worker is alive
				// check buildability of location
			}
		}

		// check the build queue
		if (build_order_queue_.nqueued() > 0
			&& !construction
			&& build_order_queue_[0].supply <= BWAPI::Broodwar->self()->supplyUsed()
			) {

			const TUnit & bu = build_order_queue_[0].unit;



			if (bu.getType() == WeaverTypes::Unit_t) {
				if (bu.getUnit().mineralPrice() <= BWAPI::Broodwar->self()->minerals()
					&& bu.getUnit().gasPrice() <= BWAPI::Broodwar->self()->gas()
					) {
					std::stringstream msg;
					msg << "Looking to build " << bu.getUnit().getName() << " " << build_order_queue_[0].supply << " " << BWAPI::Broodwar->self()->supplyUsed()
						<< " " << bu.getUnit().mineralPrice() << " " << BWAPI::Broodwar->self()->minerals();
					Logger::instance()->log(name().c_str(), msg.str().c_str());

					// build the building
					build_prep_struct bs = building(bu.getUnit());
					build_order_queue_.pop();
					build_prep_queue_.push(bs);
				}
			}
			else if (bu.getType() == WeaverTypes::Upgrade_t) {

			}
			else if (bu.getType() == WeaverTypes::Tech_t) {

			}

		}

	}

	/**
	* add a unit to the agent's control
	*/
	virtual void addUnits(const std::vector<UnitProxy> & units) {
		for (auto u : units) {
			if (u.getUnit()->getType().isWorker())
				workers_.push_back(u);
		}
	}

	/**
	* remove a unit of a given type
	*/
	virtual UnitProxy removeUnit(const BWAPI::UnitType & ut) {
		if ( ut.isWorker() && workers_.size() > 0 ) {
			// remove a worker
			// find a worker
	// grab one going to the mineral field
			for (auto wrkr : workers_) {
				BWAPI::Unit w = wrkr.getUnit();
				if (w->isIdle()) {
					return wrkr;
				}
				else if (w->getTarget() && w->getTarget()->getType().isMineralField() && !w->isCarryingMinerals()) {
					return wrkr;
				}

			}

		}

		return UnitProxy();
	}

	/**
	* Request a unit to be trained
	*/
	inline virtual void requestUnit(const BWAPI::UnitType & ut) {

	}

	/**
	* update the units controlled by this agent
	*/
	virtual void updateUnits() {
		workers_ = umanity.getUnits(this);
	}


	/**
	* remove an item from prep queue and add it to the construction
	* collection
	*/
	void buildingConstruction(BWAPI::Unit unit) { 
		// find a unit/worker from prep queue
		const unsigned nq = build_prep_queue_.nheld();
		for (unsigned i = 0; i != nq; i++) {
			const build_prep_struct & bs = build_prep_queue_[i];
			if (unit->getType() == bs.building_type
				&& unit->getTilePosition().getApproxDistance(bs.build_tile) < 5) {
				// mask the item in the prep queue
				build_prep_queue_.mask(i);

				// add the construction to the ongoing construction queue
				build_construct_queue_.push(build_construct_struct(unit, bs.worker));
			}
			
		}
	}

	/**
	* clear item from the ongoing construction collection(vqueue)
	* after a building has been completed.
	*/
	void buildingComplete(BWAPI::Unit unit) {
		buildings_.push_back(unit);
	}

protected:

  /**
  * build a building, choose a worker(s) and location.
  */
  build_prep_struct building(BWAPI::UnitType);

  
  /**
  * train a unit
  */
  void train();

 
  /**
  * research upgrade/tech
  */
  void upgrade();


  /**
  * find the placement of a building.
  */
  void building_placement();



  /**
  * update HUD
  */
  void updateHUD();

  /**
  * load a build order
  */
  void initialize_build_queue(const std::string & name);

  /**
  * Find the nearest geyser to this base
  */
  BWAPI::TilePosition findGeyser();


private:

  // internal queue of tasks
  vqueue<build_order_struct> build_order_queue_;

  // queue of building between the order and construction
  vvec<build_prep_struct> build_prep_queue_;

  // queue of buildings under construction
  vvec<build_construct_struct> build_construct_queue_;
  
  // building locations
  vmap<BWAPI::TilePosition, BWAPI::Unit> build_map_;

  // list of units (updated from UnitManager)
 std::vector<UnitProxy> workers_;

  // building status - collection of construction status
 std::vector<BWAPI::Unit> buildings_;
  
  // tech/upgrade status - collection of ongoing upgrade status

  // unit train status - collection of ongoing training status

  // location of the base
	Turk::location loc_;
	
  // hold the current status of the agent
	Turk::status status_;

  // the race
	BWAPI::Race race_;

	// the main depot of this base
	BWAPI::Unit depot_;

	// track instance of this agent class
	static unsigned m_nBases;
	unsigned m_instance;

	// name of this agent
	std::string m_name;

	// HUD lane
	int hud_lane_;

	
};


} // end Turk namespace

#endif
