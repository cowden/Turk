#pragma once

/*********************************
* C S Cowden     19 June 2018
* The unit manager keeps track of which units are
* assigned to which other mangers.  It will have the 
* role of switching units from manager to manager.
**********************************/

#include <string>
#include <vector>
#include <queue>

#include "Common.h"
#include "HUD.h"
#include "bot.h"
#include "Logger.h"
#include "volatile_collections.h"
#include "UnitProxy.h"

namespace Turk {

/**
* argument struct for parsing commands to the agent.
*/
struct unit_args : bot_args {
};

/**
* unit request typedef to map a requested unit with a 
* particular bot.
*/
//typedef std::pair<const bot *, BWAPI::UnitType> unit_request;
struct unit_request {
	bot * producer;
	bot * requester;
	BWAPI::UnitType unit_type;

	unit_request() {}

	unit_request(bot * p, bot * r, BWAPI::UnitType ut) :
		producer(p), requester(r), unit_type(ut) { }
};

/**
* The UnitManager agent is like the HR department.
* It tracks each registered agent and assigns units to them.
* It processes requests for additional units by the agents and 
* it can reallocate units amongst the agents.
*/
class UnitManager : public bot {

public:
	/**
	* Default constructor
	*/
	inline UnitManager():bot("UnitManager") {

		// agent name
		m_name = "UnitManager";

		// initialize agent list
		agents_.resize(100);
		agent_count_ = 0;

		// get a lane in the HUD
		hud_lane_ = HUD::Instance().getLane(this);
	}

	/**
	* Delete this instance
	*/
	inline ~UnitManager() { }

	/**
	* Execute a given command encoded as an integer
	*/
	virtual int execute(int command, const bot_args & args) { return 0; }

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
	virtual void dumpModel(const model_args & argsB) {}

	/**
	* Get the agent name
	*/
	virtual inline const std::string & name() const { return m_name; }


  /**
  * process queue - to be called every frame for actions needed to take.
  */
  virtual void process();

  virtual void addUnits(const std::vector<UnitProxy> & units) { }

  virtual UnitProxy removeUnit(const BWAPI::UnitType & ut) { return UnitProxy(); }

  virtual void updateUnits() { }


  /**
  * register an agent
  */
  inline void register_agent(bot * b) {

	  std::stringstream msg;
      msg << "Loading bot " << b->name() << " of type: " << b->type() << " at: 0x" << std::hex << (int)b;
	  Turk::Logger::instance()->log(m_name.c_str(), msg.str().c_str());
	  if (agents_.size() == agent_count_) agents_.resize(2 * agent_count_);
	  agents_[agent_count_++] = b;

  }


  /**
  * return the units assigned to an agent
  */
  inline std::vector<UnitProxy> getUnits(const bot * b) {
	  std::vector<UnitProxy> units;
	  for (unsigned i = 0; i != unit_map_.size(); i++) {
		  if (unit_map_.ismask(i) && unit_map_[i].first == b)
			  units.push_back(unit_map_[i].second);
	  }
	  return units;
  }


  /**
  * Initial game setup
  */
  void initialize();

  /**
  * Assign newly created units to an agent
  */
  void onUnitCreate(BWAPI::Unit);

  /**
  * call attention to agents upon unit completion
  */
  void onUnitComplete(BWAPI::Unit);

  /**
 * Request a unit of a given type. The UnitManager
 * will find a suitable agent to fill the request
 */
  inline virtual void request(BWAPI::UnitType & t, Turk::bot * b) {
	  // if t is not in the map, throw an error
	  bool has_b = false;
	  for ( unsigned i=0; i != agent_count_; i++ )
		  if (agents_[i] == b) {
			  has_b = true;
			  break;
		  }
	  assert(has_b);

	  // find the nearest basemanager which can train the unit
	  // add the unit to the queue
	  Turk::bot * bm = findNearestAgent(BWAPI::Position(b->location()), "BaseManager");

	  unit_queue_.push(unit_request(bm,b,t));

	  // request the unit from the base manager
	  // if the unit is a worker, take one from the base manager
	  if (t.isWorker()) {
		  request(t, bm, b);
		  unit_queue_.pop();
	  }
	  else {
		  request(t, bm, b);
	  }


  }

  /**
  * Request a unit of a given type from a specific agent.
  */
  inline virtual void request(BWAPI::UnitType & t, Turk::bot * sup, Turk::bot * req) {
	  // if t or sup are not in the map, throw an error
	  bool has_sup = false;
	  bool has_req = false;
	  for (unsigned i = 0; i != agent_count_; i++) {
		  const bot * agent = agents_[i];
		  if (agents_[i] == sup)
			  has_sup = true;
		  if (agents_[i] == req)
			  has_req = true;
	  }
	  assert(has_sup);
	  assert(has_req);

	  // ask sup to remove a unit, then assign it (transfer) it to t
	  UnitProxy px = sup->removeUnit(t);
	  if (!px.is_empty()) {
		  transfer(sup, req, std::vector<UnitProxy>(1, px));
	  }
  }

  /**
  * Transfer a list of units (UnitProxy) from one agent to another.
  */
  inline virtual void transfer(Turk::bot * a, Turk::bot * b, const std::vector<UnitProxy> & units) {


	  // if a or b is not registered, throw an error
	  //assert(!unit_map_.find(a).is_empty());
	  //assert(!unit_map_.find(b).is_empty());
	  bool have_a = false;
	  bool have_b = false;
	  for (unsigned i = 0; i != agent_count_; i++) {
		  if (agents_[i] == a)
			  have_a = true;
		  else if (agents_[i] == b)
			  have_b = true;
	  }
	  assert(have_a);
	  assert(have_b);

	  // cycle over the units and move from a to b.
	  // If a does not control the unit, skip to the next
	  const unsigned nunits = units.size();
	  for (unsigned i = 0; i != nunits; i++) {
		  assert(units[i] == units[i]);
		  if (unit_map_.rfind(units[i]) == a) {
			  // swap bot pointer
			  unit_map_.set(units[i], const_cast<Turk::bot *>(b));
		  }
	  }

	  // have both bots update units
	  a->updateUnits();
	  b->updateUnits();

  }

  /**
  * Return a list of registered agents
  */
  inline const std::vector<bot *> & get_agents() const {
	  return agents_;
  }

protected:

  /**
  * process request
  */
  void process_request();

  /**
  * collect recently trained units and removed dead units
  */
  void update_unit_maps();

  /**
  * search and find the nearest registered agent to a given
  * position.
  */
  inline Turk::bot * findNearestAgent(BWAPI::Position & pos) {
	  Turk::bot * b = NULL;
	  int min_dist = INT_MAX;

	  for (unsigned i = 0; i != agent_count_; i++) {
		  int dist = pos.getApproxDistance(agents_[i]->location());
		  if (dist < min_dist) {
			  min_dist = dist;
			  b = agents_[i];
		  }
	  }
	  return b;
  }

  /**
  * search and find the nearest registered agent of the specified type to a given
  * position.
  */
  inline Turk::bot * findNearestAgent(BWAPI::Position & pos, const char t[]) {
	  Turk::bot * b = NULL;
	  int min_dist = INT_MAX;

	  for (unsigned i = 0; i != agent_count_; i++) {
		  int dist = pos.getApproxDistance(agents_[i]->location());
		  if (agents_[i]->type().compare(t) == 0 && dist < min_dist) {
			  min_dist = dist;
			  b = agents_[i];
		  }
	  }
	  return b;
  }
  

 

  /**
  * Update the HUD list of registered agents
  */
  void updateHUD();

private:

  // queue of unit requests
	vqueue<unit_request> unit_queue_;
  

  // list of all units
  std::vector<UnitProxy> units_;
 
  // list of all registered agents
  std::vector<bot *> agents_;
  unsigned agent_count_;

  // unit-agent map
  vmap<const bot *, UnitProxy> unit_map_; 


  Turk::location loc_;
  Turk::status status_;
  
  // bot name
  std::string m_name;

  // HUD lane
  int hud_lane_;

};


/**
* Define a global UnitManager object.
*/
extern UnitManager umanity;

}  // end Turk namespace
