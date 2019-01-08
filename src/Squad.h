#ifndef SQUAD_H
#define SQUAD_H


/****************************
* C S Cowden      7 September 2018
* Squad - implement the tactical control of a
* squad.  This is really not much more
* than a collection of UnitProxies.
******************************/

#include <string>
#include <vector>

#include "Common.h"
#include "bot.h"

#define SQUAD_MOVE 1
#define SQUAD_ATTACK 2

namespace Turk {


/**
* Argument struct for parsing commands to squad
*/
struct squad_args : bot_args {
	unsigned command;
	Turk::location location;
};


class Squad : public bot {
public:

  /**
  * Default constructor
  */
	inline Squad() : bot("Squad") { }

  /**
  * Delete this instance
  */
  inline ~Squad() { }

  /**
  * Execute a given command encoded as an integer
  */
  inline int execute(int command, const bot_args & args) { return 0; }

  /**
  * Execute a given command given squad_args
  */
  inline int execute(const squad_args & args) {
	  if (args.command & SQUAD_MOVE) {
		  move(args.location);
	  }

	  return 0;
  }

  /**
  * Return the bot type
  */
  virtual inline const std::string & type() const { return "Squad"; }

  /**
  * Return the location
  */
  virtual inline const Turk::location & location() const { return loc_; }

  /**
  * Return the status
  */
  virtual inline const Turk::status status() const { return status_; }

  /**
  * Load a model
  */
  virtual void loadModel(const model_args & args) { }

  /**
  * Dump the model
  */
  virtual void dumpModel(const model_args & args) { }

  /**
  * Get the model name
  */
  virtual inline const std::string & name() const { return m_name; }

  /**
  * process queue - to be called every frame for actions needed to take
  */
  inline virtual void process() { 
	  
	  // update location
	  loc_ = BWAPI::Positions::Origin;
	  const unsigned nh = units_.nheld();
	  for (unsigned i=0; i != nh; i++)
		  loc_ += units_[i].getUnit()->getPosition();

	  if ( units_.nheld() ) loc_ /= (double)units_.nheld();
	  
  }

  inline void addUnits(const std::vector<UnitProxy> & units) { 
	  for (auto u : units)
		  units_.push(u);
  }

  virtual void updateUnits() { }

  /**
  * return the size in the number of units in the squad
  */
  inline virtual unsigned size() const {
	  return units_.nheld();
  }



private:

	/**
	* Move the squad (i.e.
	* move all units to a single location)
	*/
	inline void move(const Turk::location & loc) {
		const unsigned nu = units_.nheld();
		for (unsigned i = 0; i != nu; i++)
			units_[i].getUnit()->move(loc);
	}


  Turk::location loc_;
  Turk::status status_;
  
  // track the number of squad
  static unsigned m_nSquads;
  unsigned m_instance;

  // bot name
  std::string m_name;

  // the collection of units
  Turk::vvec<Turk::UnitProxy> units_;
  unsigned nUnits_;

};

} // end Turk namespace


#endif
