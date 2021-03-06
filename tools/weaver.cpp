
#include "weaver.h"

#include <cassert>

#include "ConfigDB.h"
#include "volatile_collections.h"

using namespace Turk;



TUnitCollection Weaver::getRequirements(const TUnit & unit ) {

  TUnitCollection col;

  // check if the unit can already be produced
  // if so, return an empty collection
  if ( turk_units_.findUnit(unit) < UINT_MAX ) return col;

  // check if race is correct, return a collection of None_t
  if ( turk_race_ != unit.getRace() ) {
    col.push(TUnit());
    return col;
  }
  
  // find the index of the unit in the tech tree
  unsigned index = 0;
  /*WeaverTypes unit_type = unit.getType();
  const unsigned col = unit.getType(); 
  for ( unsigned i=0; i != tech_size_; i++ ) {
    if ( tech_map_[col*tech_size_+i] == id ) {
      index = i;
      break;
    }
  }*/

  // follow dependencies
  // if the dependency can already be produced stop on that path
  // to keep the order, do a breadth first search of dependencies
  // use a queue to track next dependency, start with index
  vqueue<unsigned> queue;
  queue.push(index);
  while ( !queue.empty() ) {

    unsigned i = queue.pop();

    for ( unsigned j=0; j != tech_size_; j++ ) {

      if ( tech_tree_[j*tech_size_+i] == 1) {

        queue.push(j);

        const unsigned req_type = tech_map_[4*tech_size_+j];
        if ( req_type == WeaverTypes::Unit_t ) {
          col.push(TUnit(BWAPI::UnitType(tech_map_[req_type*tech_size_+j])));
        } else if ( req_type == WeaverTypes::Upgrade_t ) {
          col.push(TUnit(BWAPI::UpgradeType(tech_map_[req_type*tech_size_+j])));
        } else if ( req_type == WeaverTypes::Tech_t ) {
          col.push(TUnit(BWAPI::TechType(tech_map_[req_type*tech_size_+j])));
        } 

      }
    }
  }

  return col;
   

}



void Weaver::loadTechTree() {

	// connect to configuration database
	Turk::ConfigDB *db = Turk::ConfigDB::instance();

	// read the tech tree graph table
	int nrows = 0;
	std::vector<std::vector<char *> > vres = db->query(nrows,"SELECT * FROM techtree;");

	// parse results into data
	// we are expected a large square matrix of 0's and 1's
	// assert it is a square matrix
	assert(nrows == vres.size());
	assert(nrows == vres[0].size());

	tech_size_ = nrows;

	// resize the data matrix
	tech_tree_.resize(tech_size_*tech_size_);

	for (unsigned i = 0; i != nrows; i++) {
		for (unsigned j = 0; j != nrows; j++) {
			tech_tree_[i*nrows + j] = atoi(vres[i][j]);
		}
	}


}


void Turk::Weaver::loadTechMap() {

	// connect to configuration database
	Turk::ConfigDB *db = Turk::ConfigDB::instance();

	// read the tech tree graph table
        // Here we have to select unit, upgrade, tech columns
        // to correspond to the WeaverTypes enumeration
	int nrows = 0;
	std::vector<std::vector<char *> > vres = db->query(nrows,"SELECT unit,upgrade,tech,level,type FROM techmap;");

	// parse results into data
	assert(5 == vres.size());
        assert(vres[0].size() == tech_size_);

	// resize the data matrix
	tech_map_.resize(5*tech_size_);

	for (unsigned i = 0; i != 5; i++) {
		for (unsigned j = 0; j != tech_size_; j++) {
			tech_map_[i*tech_size_ + j] = atoi(vres[i][j]);
		}
	}

}


void Turk::Weaver::updateTurkTree(const TUnit & unit ) { 
  
  // make some basic checks first
  // is the unit correct for the player's race
  // has the unit already been detected
  if ( turk_race_ != unit.getRace() ) return;

  if ( turk_units_.findUnit(unit) < UINT_MAX ) return;

  // find index into tech tree
  unsigned index = 0;
  //Turk::WeaverTypes unit_type = unit.getType();
  const unsigned id = unit.getID();
  const unsigned col = unit.getType();
  for ( unsigned i=0; i != tech_size_; i++ ) {
    if ( tech_map_[col*tech_size_+i] == id ) {
      index = i;
      break;
    }
  }

  // add link in tree

  // update list of producable 
  // cycle down the column and add new units to the list
  for ( unsigned i=0; i != tech_size_; i++ ) {

    if ( tech_tree_[index*tech_size_+i] == 1 ) {

      const unsigned new_type = tech_map_[4*tech_size_+i];
      assert(new_type < 4);

      // add the unit to the unit list of available units
      if ( new_type == Turk::WeaverTypes::Unit_t ) {
		  const unsigned indx = new_type * tech_size_ + i;
		  const unsigned uti = tech_map_[indx];
        TUnit new_unit = TUnit(BWAPI::UnitType(uti));
        if ( !canTurkProduce(new_unit) ) turk_units_.push(new_unit);

      } else if ( new_type == Turk::WeaverTypes::Upgrade_t ) {

		  const unsigned indx = new_type * tech_size_ + i;
		  const unsigned uti = tech_map_[indx];
        TUnit new_unit = TUnit(BWAPI::UpgradeType(uti));
        if ( !canTurkProduce(new_unit) ) turk_units_.push(new_unit);

      } else if ( new_type == Turk::WeaverTypes::Tech_t ) {

		  const unsigned indx = new_type * tech_size_ + i;
		  const unsigned uti = tech_map_[indx];
        TUnit new_unit = TUnit(BWAPI::TechType(uti));
        if ( !canTurkProduce(new_unit) ) turk_units_.push(new_unit);

      }
    } 
  }

}


void Turk::Weaver::updateEnemyTree(const TUnit & unit) {

  // make some basic checks first
  // is the unit correct for the player's race
  // has the unit already been detected
  if ( !enemy_race_known_ || enemy_race_ != unit.getRace() ) return;


  // update list of observed units
  if ( observed_enemy_units_.findUnit(unit) == UINT_MAX ) {
    observed_enemy_units_.push(unit);
  }

  if ( enemy_units_.findUnit(unit) < UINT_MAX) return;

  // find index into tech tree
  unsigned index = 0;
  //Turk::WeaverTypes unit_type = unit.getType();
  const unsigned id = unit.getID();
  const unsigned col = unit.getType();
  for ( unsigned i=0; i != tech_size_; i++ ) {
    if ( tech_map_[col*tech_size_+i] == id ) {
      index = i;
      break;
    }
  }

  // add link in tree

  // update list of producable 
  // cycle down the column and add new units to the list
  for ( unsigned i=0; i != tech_size_; i++ ) {

    if ( tech_tree_[index*tech_size_+i] == 1 ) {

      const unsigned new_type = tech_map_[4*tech_size_+i];
      assert(new_type < 4);

	  const unsigned indx = new_type * tech_size_ + i;
	  const unsigned uti = tech_map_[indx];

      // add the unit to the unit list of available units
      if ( new_type == Turk::WeaverTypes::Unit_t ) {
  
        TUnit new_unit = TUnit(BWAPI::UnitType(uti));
        if ( !canTurkProduce(new_unit) ) enemy_units_.push(new_unit);

      } else if ( new_type == Turk::WeaverTypes::Upgrade_t ) {

        TUnit new_unit = TUnit(BWAPI::UpgradeType(uti));
        if ( !canTurkProduce(new_unit) ) enemy_units_.push(new_unit);

      } else if ( new_type == Turk::WeaverTypes::Tech_t ) {

        TUnit new_unit = TUnit(BWAPI::TechType(uti));
        if ( !canTurkProduce(new_unit) ) enemy_units_.push(new_unit);

      }
    } 
  }

}



Turk::Weaver Turk::weaver;
