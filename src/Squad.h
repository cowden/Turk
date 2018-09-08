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
#include "ArmyManager.h"

namespace Turk {

class Squad : public ArmyManager {
public:

  /**
  * Default constructor
  */
  inline Squad():ArmyManager() { }

  /**
  * Delete this instance
  */
  inline ~Squad() { }

  /**
  * Execute a given command encoded as an integer
  */
  inline int execute(int command, const bot_args & args) { return 0; }

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
  inline virtual void process() { }

  inline void addUnits(const std::vector<BWAPI::Unit> & units) { }

  virtual void updateUnits() { }



private:

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
