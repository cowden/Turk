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

	class ArmyManager : public bot {
	public:
		/**
		* Default constructor
		*/
		inline ArmyManager() {}

		/**
		* Delete this instance
		*/
		inline ~ArmyManager() { }

		/**
		* Execute a given command encoded as an integer
		*/
		virtual int execute(int command) { return 0; }

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

	protected:

	};

}  // end Turk namespace