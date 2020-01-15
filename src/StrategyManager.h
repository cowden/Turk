#pragma once

/*********************************
* C S Cowden     19 June 2018
* The strategy manager delegates and organizes the
* other managers around a high level strategy.
**********************************/

#include <string>

#include "BWAPI/Race.h"

#include "Common.h"
#include "bot.h"
#include "BaseManager.h"
#include "ArmyManager.h"
#include "UnitProxy.h"

namespace Turk {

/** 
* argument struct for parsing commands to the agent.
*/
struct strategy_args : bot_args {
	BWAPI::Race race_;
};


	class StrategyManager : public bot {
	public:
		/**
		* Default constructor
		*/
		inline StrategyManager():bot("StrategyManager"),m_name("StrategyManager"),initiate_scout_(false) {}

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
  inline virtual void process() {

	  // send command to begin scouting
	  if (!initiate_scout_ && BWAPI::Broodwar->getFrameCount() > 500 && armies_.size() > 0 ) {
		  Turk::Logger::instance()->log(name().c_str(), "Initiating Scout");
		  bot_args ba;
		  ba.command = INITIATE_SCOUT;
		  armies_[0]->execute(INITIATE_SCOUT, ba);
		  initiate_scout_ = true;
	  }


    // simply call process on all base and army managers
	  for ( auto base : bases_) base->process();

	  for ( auto army :armies_) army->process();

  }


  /**
  * Initialize the strategy
  *  - initialize the base
  */
  inline void initialize(BWAPI::Race trace, BWAPI::Race erace) {

	  // load initial base manager
	  bases_.push_back(new BaseManager());
	  bases_[0]->setVerbosity(verbose_);
	  

	  // load initial army manager
	  armies_.push_back(new ArmyManager());
	  armies_[0]->setVerbosity(verbose_);
	  

	  // initialize unit manager
	  umanity.initialize();

	  // load models
	  bases_[0]->loadModel(model_args(trace, erace, 0));
	  armies_[0]->loadModel(model_args(trace, erace, 0));

  }


  virtual void addUnits(const std::vector<UnitProxy> & units) { }

  virtual UnitProxy removeUnit(const BWAPI::UnitType & ut) { return UnitProxy(); }

  virtual void updateUnits() { }


protected:

 


private:

  // list of BaseManagers
  std::vector<BaseManager *> bases_;

  // list of ArmyManager
  std::vector<ArmyManager *> armies_;

  // reference to ARTIE

  // reference to WEAVER

  // reference to CAMERON


  Turk::location loc_;
  Turk::status status_;

  std::string m_name;

  bool initiate_scout_;

	};

}  // end Turk namespace#pragma once
