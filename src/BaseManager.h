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

#include "UnitManager.h"



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
* Agent which implements models to operate a single base.
*/
class BaseManager : public bot {
public:
	/**
	* Default constructor
	*/
	inline BaseManager():bot("BaseManager") {

		// get name and increment agent count
		m_instance = m_nBases++;
		m_name = "BaseManager_" + std::to_string(m_instance);

		// register agent with unit manager
		umanity.register_agent(this);

		std::stringstream msg;
		msg << "Creating agent ";
		msg << name() << " of type: " << type() << " at: 0x" << std::hex << (int)this;
		Turk::Logger::instance()->log(name().c_str(), msg.str().c_str());
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
		// check for IRQ commands

		// get more workers if necessary
		if (BWAPI::Broodwar->self()->minerals() >= 100
			&& depot_->getTrainingQueue().size() < 5
			&& umanity.getUnits(this).size() < 100
			) {
			depot_->train(race_.getWorker());
		}

		// check for idle workers
		BWAPI::Unit tunit = depot_->getBuildUnit();
		if (tunit) workers_.push_back(tunit);
		for (auto u : workers_) {
			if (u->isIdle()) {
				u->gather(u->getClosestUnit(BWAPI::Filter::IsMineralField));
			}
		}

		// check the build queue
		if (build_queue_.nqueued() > 0
			&& build_queue_[0].supply <= BWAPI::Broodwar->self()->supplyUsed()
			) {

			const TUnit & bu = build_queue_[0].unit;

			if (bu.getType() == WeaverTypes::Unit_t) {
				if (bu.getUnit().mineralPrice() <= BWAPI::Broodwar->self()->minerals()
					&& bu.getUnit().gasPrice() <= BWAPI::Broodwar->self()->gas()
					) {
					// build the building
					building(bu.getUnit());
				}
			}
			else if (bu.getType() == WeaverTypes::Upgrade_t) {

			}
			else if (bu.getType() == WeaverTypes::Tech_t) {

			}

		}

	}


protected:

  /**
  * build a building, choose a worker(s) and location.
  */
  void building(BWAPI::UnitType);

  
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
  void HUD();

  /**
  * load a build order
  */
  void initialize_build_queue(const std::string & name);


private:

  // internal queue of tasks
  vqueue<build_order_struct> build_queue_;

  // list of units (updated from UnitManager)
  std::vector<BWAPI::Unit> workers_;

  // building status - collection of construction status
  
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

	
};


} // end Turk namespace

#endif
