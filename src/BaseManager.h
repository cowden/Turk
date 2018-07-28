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
	inline virtual void process() { }


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

  // list of units (updated from UnitManager)

  // building status - collection of construction status
  
  // tech/upgrade status - collection of ongoing upgrade status

  // unit train status - collection of ongoing training status

  // location of the base
	Turk::location loc_;

  // hold the current status of the agent
	Turk::status status_;

  // the race
	BWAPI::Race race_;

	// track instance of this agent class
	static unsigned m_nBases;
	unsigned m_instance;

	// name of this agent
	std::string m_name;

	
};


} // end Turk namespace

#endif
