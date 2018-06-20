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

}  // end Turk namespace#pragma once
