#ifndef WEAVER_H
#define WEAVER_H

/***********************************
* C S Cowden          June 28, 2018
* Turk Tech Tree
* WEAVER: http://terminator.wikia.com/wiki/Catherine_Weaver
***********************************/


#include <vector>
#include <algorithm>
#include <climits>

#include "BWAPI/UnitType.h"
#include "BWAPI/TechType.h"
#include "BWAPI/UpgradeType.h"
#include "BWAPI/Race.h"

namespace Turk {

namespace WeaverTypes {
  enum types_t {
    Unit_t
    ,Upgrade_t
    ,Tech_t
    ,None_t
    ,Max_t
  };
}

/**
* Encapsulate 
*  * UnitType
*  * UpgradeType
*  * TechType
* into a single type.  
*/
class TUnit {
public:

  /**
  * Initialize to None types for 
  * race, unit, upgrade, tech.
  */
  TUnit(): race_(BWAPI::Races::Enum::None)
    ,unit_(BWAPI::UnitTypes::Enum::None)
    ,upgrade_(BWAPI::UpgradeTypes::Enum::None)
    ,tech_(BWAPI::TechTypes::Enum::None)
    ,level_(0)
    ,type_(WeaverTypes::None_t)
    ,id_(UINT_MAX) {}

  /**
  * Initialize to a unit 
  */
  TUnit(const BWAPI::UnitType & t): race_(t.getRace())
    ,unit_(t)
    ,upgrade_(BWAPI::UpgradeTypes::Enum::None)
    ,tech_(BWAPI::TechTypes::Enum::None)
    ,level_(0)
    ,type_(WeaverTypes::Unit_t)
    ,id_(t.getID()) { }

  /**
  * Initialize to an upgrade
  */
  TUnit(const BWAPI::UpgradeType & t): race_(t.getRace())
    ,unit_(BWAPI::UnitTypes::Enum::None)
    ,upgrade_(t)
    ,tech_(BWAPI::TechTypes::Enum::None) 
    ,level_(0)
    ,type_(WeaverTypes::Upgrade_t)
    ,id_(t.getID()) { }

  /**
  * Initialize to an upgrade and level
  */
  TUnit(const BWAPI::UpgradeType & t, int level): race_(t.getRace())
    ,unit_(BWAPI::UnitTypes::Enum::None)
    ,upgrade_(t)
    ,tech_(BWAPI::TechTypes::Enum::None)
    ,level_(level)
    ,type_(WeaverTypes::Upgrade_t)
    ,id_(t.getID()) { }

  /** 
  * Initialize to a tech
  */
  TUnit(const BWAPI::TechType & t): race_(t.getRace())
    ,unit_(BWAPI::UnitTypes::Enum::None)
    ,upgrade_(BWAPI::UpgradeTypes::Enum::None)
    ,tech_(t) 
    ,level_(0)
    ,type_(WeaverTypes::Tech_t)
    ,id_(t.getID()) {}


  /**
  * Initialize from UnitTypes enum
  */
  TUnit(BWAPI::UnitTypes::Enum::Enum t):
    unit_(t)
    ,upgrade_(BWAPI::UpgradeTypes::Enum::None)
    ,tech_(BWAPI::TechTypes::Enum::None) 
    ,level_(0)
    ,type_(WeaverTypes::Unit_t)
  {
    race_ = unit_.getRace(); 
    id_ = unit_.getID();
  }


  /**
  * Initialize from UpgradeTypes enum
  */
  TUnit(BWAPI::UpgradeTypes::Enum::Enum t):
    unit_(BWAPI::UnitTypes::Enum::None)
    ,upgrade_(t)
    ,tech_(BWAPI::TechTypes::Enum::None) 
    ,level_(0)
    ,type_(WeaverTypes::Upgrade_t)
  {
    race_ = upgrade_.getRace();
    id_ = upgrade_.getID();
  }


  /**
  * Initialize from UpgradeTypes enum and level
  */
  TUnit(BWAPI::UpgradeTypes::Enum::Enum t,int level):
    unit_(BWAPI::UnitTypes::Enum::None)
    ,upgrade_(t)
    ,tech_(BWAPI::TechTypes::Enum::None) 
    ,level_(level)
    ,type_(WeaverTypes::Upgrade_t)
  {
    race_ = upgrade_.getRace();
    id_ = upgrade_.getID();
  }

  /**
  * Initialize from TechTypes enum
  */
  TUnit(BWAPI::TechTypes::Enum::Enum t):
    unit_(BWAPI::UnitTypes::Enum::None)
    ,upgrade_(BWAPI::UpgradeTypes::Enum::None)
    ,tech_(t)
    ,level_(0)
    ,type_(WeaverTypes::Tech_t)
  {
    race_ = tech_.getRace();
    id_ = tech_.getID();
  }
    

  // ---- accesor methods ----
  // get race
  inline const BWAPI::Race & getRace() const { return race_; }

  // get unit
  inline const BWAPI::UnitType & getUnit() const { return unit_; }

  // get upgrade
  inline const BWAPI::UpgradeType & getUpgrade() const { return upgrade_; }

  // get tech
  inline const BWAPI::TechType & getTech() const { return tech_; }

  // get type
  inline WeaverTypes::types_t getType() const { return type_; }

  // get the upgrade level
  inline int getLevel() const { return level_; }

  // return the id
  inline unsigned getID() const { return id_; }


  /**
  * equality operator
  */
  inline bool operator==(const TUnit & t) const { return race_ == t.race_ && unit_ == t.unit_ && upgrade_ == t.upgrade_ && tech_ == t.tech_ && type_ == t.type_ && level_ == t.level_; }

private:

  BWAPI::Race race_;
  BWAPI::UnitType unit_;
  BWAPI::UpgradeType upgrade_;
  BWAPI::TechType tech_;

  int level_; // level of upgrade
  WeaverTypes::types_t type_;

  unsigned id_;

};


/**
* TUnitcollection
* Collection of TUnits, provides a map
* of unique key for TUnit to various BWAPI Enum
* types.
*/
class TUnitCollection {
public:

  /**
  * default constructor
  */
  TUnitCollection():index_(0U) { }

  /**
  * copy constructor
  */
  TUnitCollection(const TUnitCollection & t): tunits_(t.tunits_), index_(t.index_) { }

  //

  /**
  * add a tunit to the collection.
  */
  inline void push(const TUnit & u) 
  { 
    // check that units are unique
    //if ( std::find(tunits_.begin(),tunits_.end(),u) != tunits_.end() ) {
    if ( true ) {
      tunits_.push_back(u); 
      index_++;
    
      if ( u.getType() == WeaverTypes::Unit_t )  {
        units_.push_back(index_);
      } else if ( u.getType() == WeaverTypes::Upgrade_t ) {
        upgrades_.push_back(index_);
      } else if ( u.getType() == WeaverTypes::Tech_t ) {
        techs_.push_back(index_);
      } 
    }
  }


  /**
  * return the number of units in the collection.
  */
  inline unsigned size() const { return tunits_.size(); }

  /**
  * access the vector of tunits.
  */
  inline const TUnit & operator[](const unsigned i) const { return tunits_[i]; }


  /**
  * find matching unit
  */
  inline unsigned findUnit(const TUnit & t) const 
  {
    if ( t.getType() == WeaverTypes::Unit_t ) {
      // cycle over units to find match
      const unsigned nunits = units_.size();
      for ( unsigned i=0; i != nunits; i++ ) {
        const unsigned indx = units_[i];
        if ( t == tunits_[indx] ) return indx;
      }
    } else if ( t.getType() == WeaverTypes::Upgrade_t ) {
      // cycle over upgrades to find match
      const unsigned nups = upgrades_.size();
      for ( unsigned i=0; i != nups; i++ ) {
        const unsigned indx = upgrades_[i];
        if ( t == tunits_[indx] ) return indx;
      }
    } else if ( t.getType() == WeaverTypes::Tech_t ) {
      // cycle over techs to find match
      const unsigned ntechs = techs_.size();
      for ( unsigned i=0; i != ntechs; i++ ) {
        const unsigned indx = techs_[i];
        if ( t == tunits_[indx] ) return indx;
      }
    }

    return UINT_MAX;
  }

private:

  std::vector<TUnit> tunits_;
  
  unsigned index_;
  std::vector<unsigned> units_;
  std::vector<unsigned> upgrades_;
  std::vector<unsigned> techs_;

};


/**
* Encapsulate the BW tech tree.
* Provide some methods to navigate the full tech tree
* including upgrades.
*/
class Weaver {
public:

        Weaver():enemy_race_known_(false) { }

        // -------- accessor methods ----------
        /**
        * Determine the ordered set of requirements to produce
        * a particular unit.  Return an empty set
        * if the unit can already be produced.
        * For example, Terran_Marine -> Barracks (assuming there are SCVs and a command center.)
        */
        TUnitCollection getRequirements(const TUnit &);
 
        /**
        * Search for a Unit type by a given string.
        */ 
	inline TUnit getUnitType(const std::string & str) {
		// cycle over all units.
		for (auto unit : BWAPI::UnitTypes::allUnitTypes()) {
			if (unit.getName() == str) {
				return TUnit(unit);
			}
		}

		return TUnit(BWAPI::UnitTypes::None);
	}

	/**
	* Can Turk produce a unit?
	*/
        bool canTurkProduce(const TUnit & unit ) {
          // cycle over units
          return turk_units_.findUnit(unit) < UINT_MAX;
        }

	/**
	* Can enemy produce a unit?
	*/
        bool canEnemyProduce(const TUnit & unit) {
          // cycle over enemy units
          return enemy_units_.findUnit(unit) < UINT_MAX;
        }

      /**
      * Have we observed a particular unit
      */
      bool observedEnemyUnit(const TUnit & unit) {
        // cycle over observed enemy units
        return observed_enemy_units_.findUnit(unit) < UINT_MAX;
      }


        // ----- updating methods --------

        /**
        *  Load the tech tree data
        */
        inline void load() {
          loadTechTree();
          loadTechMap();
        }

        /**
        * Set the turk race
        */
        inline void setRace(BWAPI::Race race) { turk_race_ = race; }

        /**
        * Set enemy race
        */
        inline void setEnemyRace(BWAPI::Race race) {
          enemy_race_known_ = true;
          enemy_race_ = race;
        }


	/**
	* Update progress in Turk techtree
	* i.e. upon building complete or upgrade
	*/
        void updateTurkTree(const TUnit & );

	/**
	* Update enemy techtree by 
	* spotting  enemy units and buildings.
	*/
        void updateEnemyTree(const TUnit & );


protected:

	/**
	* Load the tech tree
	*/
	void loadTechTree();

        /**
        * Load the tech map
        */
        void loadTechMap();


private:

	// techtree
	std::vector<unsigned> tech_tree_;
	unsigned tech_size_;

        std::vector<unsigned> tech_map_;

	// The Turk's tech tree (i.e. units it has)
        std::vector<unsigned> turk_tree_;

        // list of Turk's available units to produce	
        TUnitCollection turk_units_;

	// enemy tech trees (i.e. units it has)
        std::vector<unsigned> enemy_tree_;

        // list of enemy available units to produce
        TUnitCollection enemy_units_;

        // list of observed enemy units
        TUnitCollection observed_enemy_units_;

        //
        BWAPI::Race turk_race_;
        bool enemy_race_known_;
        BWAPI::Race enemy_race_;


};

/**
* Declare a global Weaver object
*/
extern Weaver weaver;

} // end Turk namespace

#endif
