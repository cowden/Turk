#pragma once

/*********************************
* C S Cowden     19 June 2018
* The strategy manager delegates and organizes the
* other managers around a high level strategy.
**********************************/

#include <string>

#include "Common.h"
#include "bot.h"

namespace Turk {

/** 
* argument struct for parsing commands to the agent.
*/
struct strategy_args : bot_args {
};


	class StrategyManager : public bot {
	public:
		/**
		* Default constructor
		*/
		inline StrategyManager() {}

		/**
		* Delete this instance
		*/
		inline ~StrategyManager() { }

		/**
		* Execute a given command encoded as an integer
		*/
		virtual int execute(int command,const bot_args & args) { return 0; }

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
  * Initialize the strategy
  *  - load the UnitManager
  *  - initialize the base
  */
  void initialize();


private:

  // reference to the UnitManager
  UnitManager & unit_manager_;

  // list of BaseManagers
  std::vector<BaseManager> bases_;

  // list of ArmyManager
  std::vector<ArmyManager> armies_;

  // reference to ARTIE

  // reference to WEAVER

  // reference to CAMERON

	};

}  // end Turk namespace#pragma once
