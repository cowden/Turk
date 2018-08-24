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

/**
* The base struct by which to pass arguments
* to inherited classes.
*/
struct bot_args {};

/**
* The base struct by which to pass arguments
* for the loading and dumping of models
*/
struct model_args {
	BWAPI::Race trace_; // This bot's race
	BWAPI::Race erace_; // main enemy race (main in case of multiple. add vector later)
	int index_; // index model

	// constructor
	model_args(BWAPI::Race t, BWAPI::Race e, int i) : trace_(t), erace_(e), index_(i) { }
};


/**
* abstract base class for sub-agents.
*/
class bot {

public:

	/**
	* initialize the type
	*/
	bot(const std::string & t):type_(t) { }

	/**
	* Execute a given command encoded as an integer
	*/
	virtual int execute(int command, const bot_args &) = 0;

	/**
	* Return the bot type
	*/
	virtual const std::string & type() const {
		return type_;
	}

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
	virtual void loadModel(const model_args &) = 0;

	/**
	* Dump the model
	*/
	virtual void dumpModel(const model_args &) = 0;

	/**
	* Delete this instance
	*/
	inline ~bot() { }

	/**
	* Get the model name
	*/
	virtual const std::string & name() const = 0;


  /**
  * process queue - to be called every frame for actions needed to take.
  */
  virtual void process() = 0;

  /**
  * add units to bot control
  */
  virtual void addUnits(const std::vector<BWAPI::Unit> &) = 0;

  /**
  * update unit list under bot control
  */
  virtual void updateUnits() = 0;

private:

	std::string type_;


};

} // end Turk namespace


#endif
