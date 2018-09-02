#pragma once

/*********************************
* C S Cowden     19 June 2018
* The unit manager keeps track of which units are
* assigned to which other mangers.  It will have the 
* role of switching units from manager to manager.
**********************************/

#include <string>
#include <vector>
#include <queue>

#include "Common.h"
#include "bot.h"
#include "Logger.h"
#include "volatile_collections.h"

namespace Turk {

/**
* argument struct for parsing commands to the agent.
*/
struct unit_args : bot_args {
};

/**
* unit request typedef to map a requested unit with a 
* particular bot.
*/
typedef std::pair<const bot *, BWAPI::UnitType> unit_request;

/**
* The UnitManager agent is like the HR department.
* It tracks each registered agent and assigns units to them.
* It processes requests for additional units by the agents and 
* it can reallocate units amongst the agents.
*/
class UnitManager : public bot {

public:
	/**
	* Default constructor
	*/
	inline UnitManager():bot("UnitManager") {

		// agent name
		m_name = "UnitManager";

		// initialize agent list
		agents_.resize(100);
		agent_count_ = 0;
	}

	/**
	* Delete this instance
	*/
	inline ~UnitManager() { }

	/**
	* Execute a given command encoded as an integer
	*/
	virtual int execute(int command, const bot_args & args) { return 0; }

	/**
	* Return the bot type
	*/
	virtual const std::string & type() const { return "0"; }

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
	virtual void dumpModel(const model_args & argsB) {}

	/**
	* Get the agent name
	*/
	virtual inline const std::string & name() const { return m_name; }


  /**
  * process queue - to be called every frame for actions needed to take.
  */
  virtual void process();

  virtual void addUnits(const std::vector<BWAPI::Unit> & units) { }

  virtual void updateUnits() { }


  /**
  * register an agent
  */
  inline void register_agent(bot * b) {

	  std::stringstream msg;
      msg << "Loading bot " << b->name() << " of type: " << b->type() << " at: 0x" << std::hex << (int)b;
	  Turk::Logger::instance()->log(m_name.c_str(), msg.str().c_str());
	  if (agents_.size() == agent_count_) agents_.resize(2 * agent_count_);
	  agents_[agent_count_++] = b;

  }


  /**
  * return the units assigned to an agent
  */
  inline std::vector<BWAPI::Unit> getUnits(const bot * b) {
	  std::vector<BWAPI::Unit> units;
	  for (unsigned i = 0; i != unit_map_.size(); i++) {
		  if (unit_map_.ismask(i) && unit_map_[i].first == b)
			  units.push_back(unit_map_[i].second);
	  }
	  return units;
  }


  /**
  * Initial game setup
  */
  void initialize();

  /**
  * Assign newly created units to an agent
  */
  void onUnitCreate(BWAPI::Unit);

  /**
  * call attention to agents upon unit completion
  */
  void onUnitComplete(BWAPI::Unit);

protected:

  /**
  * process request
  */
  void process_request();

  /**
  * collect recently trained units and removed dead units
  */
  void update_unit_maps();

  /**
  * search and find the nearest registered agent to a given
  * position.
  */
  inline Turk::bot * findNearestAgent(BWAPI::Position & pos) {
	  Turk::bot * b = NULL;
	  int min_dist = INT_MAX;

	  for (unsigned i = 0; i != agent_count_; i++) {
		  int dist = pos.getApproxDistance(agents_[i]->location());
		  if (dist < min_dist) {
			  min_dist = dist;
			  b = agents_[i];
		  }
	  }
	  return b;
  }

  /**
  * search and find the nearest registered agent of the specified type to a given
  * position.
  */
  inline Turk::bot * findNearestAgent(BWAPI::Position & pos, const char t[]) {
	  Turk::bot * b = NULL;
	  int min_dist = INT_MAX;

	  for (unsigned i = 0; i != agent_count_; i++) {
		  int dist = pos.getApproxDistance(agents_[i]->location());
		  if (agents_[i]->type().compare(t) == 0 && dist < min_dist) {
			  min_dist = dist;
			  b = agents_[i];
		  }
	  }
	  return b;
  }
  

private:

  // queue of unit requests
	std::queue<unit_request> unit_queue_;
  

  // list of all units
  std::vector<BWAPI::Unit> units_;
 
  // list of all registers agents
  std::vector<bot *> agents_;
  unsigned agent_count_;

  // unit-agent map
  vmap<const bot *, BWAPI::Unit> unit_map_;
  

  Turk::location loc_;
  Turk::status status_;
  
  // bot name
  std::string m_name;

};


/**
* Define a global UnitManager object.
*/
extern UnitManager umanity;

}  // end Turk namespace
