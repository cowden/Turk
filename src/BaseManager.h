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

namespace Turk {

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

} // end Turk namespace

#endif
