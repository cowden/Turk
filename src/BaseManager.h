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
* Agent which implements models to operate a single base.
*/
class BaseManager : public bot {
public:
	/**
	* Default constructor
	*/
	inline BaseManager() {}

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
	virtual const std::string & type() const { return "0"; }

	/**
	* Return the location
	*/
	virtual const location & location() const { return loc; }

	/**
	* Return the status
	*/
	virtual const status status() const { return status_; }

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
  * build a building, choose a worker(s) and location.
  */
  void building();

  
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




private:

  // internal queue of tasks

  // UnitManager

  // list of units (updated from UnitManager)

  // building status - collection of construction status
  
  // tech/upgrade status - collection of ongoing upgrade status

  // unit train status - collection of ongoing training status
	
};

} // end Turk namespace

#endif
