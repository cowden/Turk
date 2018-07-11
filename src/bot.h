#ifndef BOT_H
#define BOT_H



/****************************************
* C S Cowden  11 June 2018
* The bot base class for the various sub-agents
* implemented in the TURK.
******************************************/


#include <string>

#include "Common.h"

namespace Turk {

	class status;

/**
* The base struct by which to pass arguments
* to inherited classes.
*/
struct bot_args {};


/**
* abstract base class for sub-agents.
*/
class bot {

public:

	/**
	* Execute a given command encoded as an integer
	*/
	virtual int execute(int command, const bot_args &) = 0;

	/**
	* Return the bot type
	*/
	virtual const std::string & type() const = 0;

	/**
	* Return the location
	*/
	virtual const location & location() const = 0;

	/**
	* Return the status
	*/
	virtual const status status() const = 0;

	/**
	* Load a model
	*/
	virtual void loadModel() = 0;

	/**
	* Dump the model
	*/
	virtual void dumpModel() = 0;

	/**
	* Delete this instance
	*/
	inline ~bot() { }


  /**
  * process queue - to be called every frame for actions needed to take.
  */
  virtual void process() = 0;


};

} // end Turk namespace


#endif
