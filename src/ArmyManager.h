#pragma once

/*********************************
* C S Cowden     19 June 2018
* Army Manager - manages combat units
* or other army manager units.
**********************************/

#include <string>

#include "Common.h"
#include "bot.h"

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
		inline ArmyManager():bot("ArmyManager") {}

		/**
		* Delete this instance
		*/
		inline ~ArmyManager() { }

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
		inline virtual void process() { }

		virtual void addUnits(const std::vector<UnitProxy> & units) { }

		virtual void updateUnits() { }

protected:

private:

		Turk::location loc_;

		Turk::status status_;

		// track the number of army agents
		static unsigned m_nArmies;
		unsigned m_instance;

		// bot name
		std::string m_name;
};

}  // end Turk namespace
