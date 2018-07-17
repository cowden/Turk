#pragma once

/*********************************
* C S Cowden     19 June 2018
* The unit manager keeps track of which units are
* assigned to which other mangers.  It will have the 
* role of switching units from manager to manager.
**********************************/

#include <string>

#include "Common.h"
#include "bot.h"

namespace Turk {

/**
* argument struct for parsing commands to the agent.
*/
struct unit_args : bot_args {
};

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
	inline UnitManager() {}

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
	virtual void loadModel() { }

	/**
	* Dump the model
	*/
	virtual void dumpModel() {}


  /**
  * process queue - to be called every frame for actions needed to take.
  */
  virtual void process();


protected:

  /**
  * process request
  */
  void process_request();

  /**
  * collect recently trained units and removed dead units
  */
  void update_unit_maps();


private:

  // list of all units
  std::vector<BWAPI::Unit> units_;
 
  // list of all registers agents
  std::vector<bot> agents_;

  // unit-agent map
  std::vector<unsigned> unit_agent_map_;
  std::vector<unsigned> agent_unit_map_;


  Turk::location loc_;
  Turk::status status_;

};


/**
* Define a global UnitManager object.
*/
extern UnitManager umanity;

}  // end Turk namespace
