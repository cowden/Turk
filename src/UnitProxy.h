#ifndef UNITPROXY_H
#define UNITPROXY_H

/*************************************
* C S Cowden        7 September 2018
* UnitProxy - A proxy to BWAPI::Unit.
* This class incorporates interfaces
* to navigation/path finding and control
* from the owning manager.
**************************************/

#include <string>

#include "Common.h"
#include "bot.h"

namespace Turk{

class UnitProxy : public bot {
public:
		/**
		* Default constructor
		*/
		inline UnitProxy():bot("UnitProxy"),empty_(true) {}

		/**
		* Construct from a BWAPI::Unit
		*/
		inline UnitProxy(BWAPI::Unit & u) :
			bot("UnitProxy"),empty_(false), unit_(u) { }

		/**
		* Delete this instance
		*/
		inline ~UnitProxy() { }

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

		virtual UnitProxy removeUnit(const BWAPI::UnitType & ut) { return UnitProxy(); }

		virtual void updateUnits() { }

		/**
		* Return a flag to indicate if this is an empty proxy
		*/
		inline bool is_empty() const { return empty_; }

  /**
  * Expose the BWAPI::Unit for low level controls.
  */
  BWAPI::Unit & getUnit() { return unit_; }

  /**
  * overload the equality operator
  */
  bool operator==(const UnitProxy & up) const {
	  return up.m_name == m_name && up.unit_ == unit_;
  }

protected:

private:

		Turk::location loc_;

		Turk::status status_;

		// track the number of army agents
		static unsigned m_nArmies;
		unsigned m_instance;

		// flag to test if this is an empty proxy
		bool empty_;

		// bot name
		std::string m_name;

    BWAPI::Unit unit_;
};

} // end Turk namespace

#endif
