


////////////////////////////////////////////
// C S Cowden June 20, 2018
// Standalone program to capture the broodwar 
// tech tree from BWAPI.
////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "boost/graph/graphviz.hpp"

//#include "gvc.h"

#include "UnitType.h"
#include "UnitSizeType.h"
#include "Race.h"
#include "WeaponType.h"
#include "Order.h"

#include "weaver.h"


/**
* Global variables to hold table information
*/
// ----- unit data fields ----

/**
* This class holds the unit information
*/
class UnitCollection {
public:

  /**
  * Dump unit data list.
  */
  void dump_csv(const char * name);

  /**
  * add a unit to the list of encountered units.
  */
  inline void push(const BWAPI::UnitType & unit ) { units_.push_back(unit); }

private:

  std::vector<BWAPI::UnitType> units_;

  static const unsigned n_cols_ = 70;
  static const std::string unit_cols_[n_cols_];


};

const std::string UnitCollection::unit_cols_[] = {
  "unit"
  ,"name"
  ,"race"
  ,"maxHitPoints"
  ,"maxShields"
  ,"maxEnergy"
  ,"armor"
  ,"mineralPrice"
  ,"gasPrice"
  ,"buildTime"
  ,"supplyRequried"
  ,"supplyProvided"
  ,"spaceRequired"
  ,"spaceProvided"
  ,"buildScore"
  ,"destroyScore"
  ,"size"
  ,"tileWidth"
  ,"tileHeight"
  ,"tileSize"
  ,"dimensionLeft"
  ,"dimensionRight"
  ,"dimensionUp"
  ,"dimensionDown"
  ,"width"
  ,"height"
  ,"seekRange"
  ,"sightRange"
  ,"maxGroundHits"
  ,"maxAirHits"
  ,"topSpeed"
  ,"acceleration"
  ,"haltDistance"
  ,"turnRadius"
  ,"canProduce"
  ,"canAttack"
  ,"canMove"
  ,"isFlyer"
  ,"regeneratesHP"
  ,"isSpellcaster"
  ,"hasPermanentCloak"
  ,"isInvincible"
  ,"isOrganic"
  ,"isMechanical"
  ,"isRobotic"
  ,"isDetector"
  ,"isResourceContainer"
  ,"isResourceDepot"
  ,"isRefinery"
  ,"isWorker"
  ,"requiresPsi"
  ,"requiresCreep"
  ,"isTwoUnitsInOneEgg"
  ,"isBurrowable"
  ,"isCloakable"
  ,"isBuilding"
  ,"isAddon"
  ,"isFlyingBuilding"
  ,"isNeutral"
  ,"isHero"
  ,"isPowerup"
  ,"isBeacon"
  ,"isFlagBeacon"
  ,"isSpecialBuilding"
  ,"isSpell"
  ,"producesCreep"
  ,"producesLarva"
  ,"isMineralField"
  ,"isCritter"
  ,"canBuildAddon"
};

void UnitCollection::dump_csv(const char * name ) {
  
  // open output file
  std::ofstream ofile(name,std::ofstream::out);

  // write header 
  bool isFirstColumn = true;
  for ( auto col: unit_cols_ ) {
   if ( !isFirstColumn ) ofile << ",";
   else isFirstColumn = false;
   ofile << col;
  }
  ofile << std::endl;

  // cycle over units
  for ( auto unit: units_ ) {

    // unit
    ofile << static_cast<int>(unit.getID());

    // unit name
    ofile << "," << unit.c_str();

    //"race"
    ofile << "," << unit.getRace();

    //"maxHitPoints"
    ofile << "," << unit.maxHitPoints();

    //"maxShields"
    ofile << "," << unit.maxShields();

    //"maxEnergy"
    ofile << "," << unit.maxEnergy();

    //"armor"
    ofile << "," << unit.armor();

    //"mineralPrice"
    ofile << "," << unit.mineralPrice();

    //"gasPrice"
    ofile << "," << unit.gasPrice();

    //"buildTime"
    ofile << "," << unit.buildTime();

    //"supplyRequried"
    ofile << "," << unit.supplyRequired();

    //"supplyProvided"
    ofile << "," << unit.supplyProvided();

    //"spaceRequired"
    ofile << "," << unit.spaceRequired();

    //"spaceProvided"
    ofile << "," << unit.spaceProvided();

    //"buildScore"
    ofile << "," << unit.buildScore();

    //"destroyScore"
    ofile << "," << unit.destroyScore();

    //"size"
    ofile << "," << static_cast<int>(unit.size().getID());

    //"tileWidth"
    ofile << "," << unit.tileWidth();

    //"tileHeight"
    ofile << "," << unit.tileHeight();

    //"tileSize"
    ofile << "," << unit.tileSize();

    //"dimensionLeft"
    ofile << "," << unit.dimensionLeft();

    //"dimensionRight"
    ofile << "," << unit.dimensionRight();

    //"dimensionUp"
    ofile << "," << unit.dimensionUp();

    //"dimensionDown"
    ofile << "," << unit.dimensionDown();

    //"width"
    ofile << "," << unit.width();

    //"height"
    ofile << "," << unit.height();

    //"seekRange"
    ofile << "," << unit.seekRange();

    //"sightRange"
    ofile << "," << unit.sightRange();

    //"maxGroundHits"
    ofile << "," << unit.maxGroundHits();

    //"maxAirHits"
    ofile << "," << unit.maxAirHits();

    //"topSpeed"
    ofile << "," << unit.topSpeed();

    //"acceleration"
    ofile << "," << unit.acceleration();

    //"haltDistance"
    ofile << "," << unit.haltDistance();

    //"turnRadius"
    ofile << "," << unit.turnRadius();

    //"canProduce"
    ofile << "," << unit.canProduce();

    //"canAttack"
    ofile << "," << unit.canAttack();

    //"canMove"
    ofile << "," << unit.canMove();

    //"isFlyer"
    ofile << "," << unit.isFlyer();

    //"regeneratesHP"
    ofile << "," << unit.regeneratesHP();

    //"isSpellcaster"
    ofile << "," << unit.isSpellcaster();

    //"hasPermanentCloak"
    ofile << "," << unit.hasPermanentCloak();

    //"isInvincible"
    ofile << "," << unit.isInvincible();

    //"isOrganic"
    ofile << "," << unit.isOrganic();

    //"isMechanical"
    ofile << "," << unit.isMechanical();

    //"isRobotic"
    ofile << "," << unit.isRobotic();

    //"isDetector"
    ofile << "," << unit.isDetector();

    //"isResourceContainer"
    ofile << "," << unit.isResourceContainer();

    //"isResourceDepot"
    ofile << "," << unit.isResourceDepot();

    //"isRefinery"
    ofile << "," << unit.isRefinery();

    //"isWorker"
    ofile << "," << unit.isWorker();

    //"requiresPsi"
    ofile << "," << unit.requiresPsi();

    //"requiresCreep"
    ofile << "," << unit.requiresCreep();

    //"isTwoUnitsInOneEgg"
    ofile << "," << unit.isTwoUnitsInOneEgg();

    //"isBurrowable"
    ofile << "," << unit.isBurrowable();

    //"isCloakable"
    ofile << "," << unit.isCloakable();

    //"isBuilding"
    ofile << "," << unit.isBuilding();

    //"isAddon"
    ofile << "," << unit.isAddon();

    //"isFlyingBuilding"
    ofile << "," << unit.isFlyingBuilding();

    //"isNeutral"
    ofile << "," << unit.isNeutral();

    //"isHero"
    ofile << "," << unit.isHero();

    //"isPowerup"
    ofile << "," << unit.isPowerup();

    //"isBeacon"
    ofile << "," << unit.isBeacon();

    //"isFlagBeacon"
    ofile << "," << unit.isFlagBeacon();

    //"isSpecialBuilding"
    ofile << "," << unit.isSpecialBuilding();

    //"isSpell"
    ofile << "," << unit.isSpell();

    //"producesCreep"
    ofile << "," << unit.producesCreep();

    //"producesLarva"
    ofile << "," << unit.producesLarva();

    //"isMineralField"
    ofile << "," << unit.isMineralField();

    //"isCritter"
    ofile << "," << unit.isCritter();

    //"canBuildAddon"
    ofile << "," << unit.canBuildAddon();

    ofile << std::endl;
  }

  // close output file
  ofile.close();
}


class TechCollection {
public:

  /**
  * Dump unit data list.
  */
  void dump_csv(const char * name);

  /**
  * add a unit to the list of encountered units.
  */
  inline void push(const BWAPI::TechType & tech ) { techs_.push_back(tech); }

private:

  std::vector<BWAPI::TechType> techs_;

  static const unsigned n_cols_ = 13U;
  static const std::string tech_cols_[n_cols_];

};

const std::string TechCollection::tech_cols_[] = {
  "tech"
  ,"name"
  ,"race"
  ,"mineralPrice"
  ,"gasPrice"
  ,"researchTime"
  ,"energyCost"
  ,"whatResearches"
  ,"getWeapon"
  ,"targetsUnit"
  ,"targetsPosition"
  ,"order"
  ,"requiredUnit"
};

void TechCollection::dump_csv(const char * name) {

  // open output file
  std::ofstream ofile(name,std::ofstream::out);

  // write header
  bool isFirstColumn = true;
  for ( auto col: tech_cols_ ) {
    if ( !isFirstColumn ) ofile << ",";
    else isFirstColumn = false;
    ofile << col;
  }
  ofile << std::endl;

  // cycle over tech types
  for ( auto tech: techs_ ) {

    // "tech"
    ofile << static_cast<int>(tech.getID());

    // name"
    ofile << "," << tech.c_str();

    // "race"
    ofile << "," << tech.getRace();

    // "mineralPrice"
    ofile << "," << tech.mineralPrice();

    // "gasPrice"
    ofile << "," << tech.gasPrice();

    // "researchTime"
    ofile << "," << tech.researchTime();

    // "energyCost"
    ofile << "," << tech.energyCost();

    // "whatResearches"
    ofile << "," << static_cast<int>(tech.whatResearches().getID());

    // "getWeapon"
    ofile << "," << static_cast<int>(tech.getWeapon().getID());

    // "targetsUnit"
    ofile << "," << tech.targetsUnit();

    // "targetsPosition"
    ofile << "," << tech.targetsPosition();

    // "order"
    ofile << "," << static_cast<int>(tech.getOrder().getID());

    // "requiredUnit"
    ofile << "," << static_cast<int>(tech.requiredUnit().getID()); 

    ofile << std::endl;
  }

  // close the output file
  ofile.close();

}


/**
* Upgrade collection
*/
class UpgradeCollection {
public:
  /**
  * Dump unit data list.
  */
  void dump_csv(const char * name);

  /**
  * add a unit to the list of encountered units.
  */
  inline void push(const BWAPI::UpgradeType & upgrade ) { upgrades_.push_back(upgrade); }

private:

  std::vector<BWAPI::UpgradeType> upgrades_;

  static const unsigned n_cols_ = 14U;
  static const std::string upgrade_cols_[n_cols_];
};

const std::string UpgradeCollection::upgrade_cols_[] = {
  "upgrade"
  ,"name"
  ,"level"
  ,"race"
  ,"mineralPrice"
  ,"mineralPriceFactor"
  ,"gasPrice"
  ,"gasPriceFactor"
  ,"upgradeTime"
  ,"upgradeTimeFactor"
  ,"maxRepeats"
  ,"whatUpgrades"
  ,"whatsRequired"
};


void UpgradeCollection::dump_csv(const char * name ) {

  // open file
  std::ofstream ofile(name,std::ofstream::out);

  // write header
  bool isFirstColumn = true;
  for ( auto col: upgrade_cols_ ) {
    if ( !isFirstColumn ) ofile << ",";
    else isFirstColumn = false;
    ofile << col;
  }
  ofile << std::endl;

  // write upgrades
  for ( auto upgrade: upgrades_ ) {

    const unsigned nLevels = upgrade.maxRepeats();
    for ( unsigned i=0; i != nLevels; i++ ) {

      // upgrade
      ofile << static_cast<int>(upgrade.getID());

      // name 
      ofile << "," << upgrade.c_str();

      // level
      ofile << "," << i;

      // race
      ofile << "," << upgrade.getRace();

      // mineralPrice
      ofile << "," << upgrade.mineralPrice(i);

      // mineralPriceFactor
      ofile << "," << upgrade.mineralPriceFactor();

      // gasPrice
      ofile << "," << upgrade.gasPrice(i);

      // gasPriceFactor
      ofile << "," << upgrade.gasPriceFactor();

      // upgradeTime
      ofile << "," << upgrade.upgradeTime(i);

      // upgradeTimeFactor
      ofile << "," << upgrade.upgradeTimeFactor();

      // maxRepeats
      ofile << "," << upgrade.maxRepeats();

      // whatUpgrades
      ofile << "," << static_cast<int>(upgrade.whatUpgrades());

      // whatsRequired
      ofile << "," << static_cast<int>(upgrade.whatsRequired(i));

      ofile << std::endl;

    }

  }

  // close file
  ofile.close();

}


// ----- unit list/ myunitid map ----
// ----- requirements graphs -----
class TechTree {
public:

  TechTree() { }

  /**
  * dump the technology tree matrix to a csv file.
  */
  void dump_graph(const char * name);

  /**
  * dump the TUnit (unit, upgrade, or tech) map to a csv file.
  */
  void dump_map(const char * name);

  /**
  * dump the dot file
  */
  void dump_dot(const char * name);

  void mkgraph();

  inline void push(const Turk::TUnit & t) 
  { 
    if ( t.getType() == Turk::WeaverTypes::Upgrade_t ) {
      const unsigned nUpgrades = t.getUpgrade().maxRepeats();
      
      for ( unsigned i=0; i != nUpgrades; i++ ) {
        tunits_.push(Turk::TUnit(t.getUpgrade(),i));
      }

    }
    else
      tunits_.push(t); 
  }

private:

  Turk::TUnitCollection tunits_;
  std::vector<int> graph_;

  static const unsigned n_cols_ = 7U;
  static const std::string tech_cols_[n_cols_];


  void dump_subgraph(std::ofstream & ofile, const BWAPI::Race & race);

  std::string dump_node_label(const unsigned i);

};

const std::string TechTree::tech_cols_[] = {
  "techtree"
  ,"race"
  ,"unit"
  ,"upgrade"
  ,"tech"
  ,"level"
  ,"type"
};

void TechTree::mkgraph() {

  // resize the graph array
  const unsigned size_ = tunits_.size();
  graph_.resize(size_*size_);

  // cycle over all of the tunits in the collection
  for ( unsigned i=0; i != size_; i++ ) {
    const Turk::TUnit & tu = tunits_[i];

    // process unit requirements
    if ( tu.getType() == Turk::WeaverTypes::Unit_t ) {
      const BWAPI::UnitType & unit = tu.getUnit();

      for ( auto r: unit.requiredUnits() ) {
        // find required unit
        const Turk::TUnit ru(r.first);
        const unsigned indx = tunits_.findUnit(ru);
        if ( indx < UINT_MAX) graph_[i*size_+indx] = r.second;
      }

      // tech requirements
      const BWAPI::TechType & rtech = unit.requiredTech();
      const Turk::TUnit trt(rtech);
      const unsigned tindx = tunits_.findUnit(trt);
      if ( tindx < UINT_MAX) graph_[i*size_+tindx] = 1;

      // find out what upgrades this unit does
      for ( auto u: unit.upgradesWhat() ) {
        const Turk::TUnit ru(u);
        const unsigned indx = tunits_.findUnit(ru);
        if ( indx < UINT_MAX ) graph_[indx*size_+i] = 1;
      }

      // find out what tech this unit researches
      for ( auto t: unit.researchesWhat() ) {
        const Turk::TUnit tu(t);
        const unsigned indx = tunits_.findUnit(tu);
        if ( indx < UINT_MAX ) graph_[indx*size_+i] = 1;
      }

    } else if ( tu.getType() == Turk::WeaverTypes::Upgrade_t ) {

      // process upgrade requirements
      const BWAPI::UpgradeType & upgrade = tu.getUpgrade();
      const unsigned level = tu.getLevel();

      const BWAPI::UnitType & ru = upgrade.whatsRequired(level);
      const unsigned indx = tunits_.findUnit(Turk::TUnit(ru));
      if ( indx < UINT_MAX) graph_[i*size_+indx] = 1;

      if ( level > 0 ) {
        const unsigned nl = level - 1U;
        const unsigned l1indx = tunits_.findUnit(Turk::TUnit(upgrade,nl));
        if ( indx < UINT_MAX ) graph_[i*size_+l1indx] = 1;
      }


    } else if ( tu.getType() == Turk::WeaverTypes::Tech_t ) {

      // process tech requirements
      const BWAPI::TechType & tech = tu.getTech();

      const BWAPI::UnitType & ru = tech.requiredUnit();
      
      const unsigned indx = tunits_.findUnit(Turk::TUnit(ru));
      if ( indx < UINT_MAX ) graph_[i*size_+indx] = 1;

    }

  }

}

void TechTree::dump_map(const char * name ) {

  // open the file
  std::ofstream ofile(name,std::ofstream::out);

  // write the header
  for ( unsigned i=0; i != n_cols_; i++ ) {
    if ( i != 0 ) ofile << ",";
    ofile << tech_cols_[i];
  }
  ofile << std::endl;

  // write data
  const unsigned size = tunits_.size();
  for ( unsigned i=0; i != size; i++ ) {
    // techtree index
    ofile << i;

    const Turk::TUnit & unit = tunits_[i];
    // race
    ofile << "," << unit.getRace();
    
    // unit
    ofile << "," << static_cast<int>(unit.getUnit().getID());
  
    // upgrade
    ofile << "," << static_cast<int>(unit.getUpgrade().getID());

    // tech
    ofile << "," << static_cast<int>(unit.getTech().getID());
 
    // level
    ofile << "," << unit.getLevel();

    // type
    ofile << "," << static_cast<int>(unit.getType());
  }
  ofile << std::endl;

  // close the file
  ofile.close();

}


void TechTree::dump_graph(const char * name ) {

  std::ofstream ofile(name,std::ofstream::out);

  const unsigned size_ = tunits_.size();

  for ( unsigned i=0; i != size_; i++ ) {
    bool isFirst = true;
    for ( unsigned j=0; j != size_; j++ ) {
      if ( !isFirst ) ofile << ",";
      else isFirst = false;

      ofile << graph_[i*size_+j]; 
    }
    
    ofile << std::endl;
  }

  ofile.close();
}



void TechTree::dump_subgraph(std::ofstream & ofile, const BWAPI::Race & race) {

  // write header 
  ofile << "subgraph cluster_" << race.getID() << " {" << std::endl;

  // collect nodes
  std::vector<int> nodes;
  const unsigned N = tunits_.size();

  // collect edges
  std::vector<std::pair<int,int> > edges;

  for ( unsigned i = 0; i != N; i++ ) {
    const Turk::TUnit & unit = tunits_[i];
    if ( unit.getRace() == race ) {
      nodes.push_back(i);

      for ( unsigned j = 0; j != N; j++ ) {
        if ( graph_[j*N+i] == 1 ) edges.push_back( std::pair<int,int>(i,j) );
      }
    }
  }
  

  // write nodes
  const unsigned nNodes = nodes.size();
  for ( unsigned i=0; i != nNodes; i++ ) {
    ofile << "  " << nodes[i] << " " << dump_node_label(nodes[i]) << ";" << std::endl;
  }

  // write edges
  const unsigned nEdges = edges.size();
  for ( unsigned i=0; i != nEdges; i++ ) {
    const std::pair<int,int> & edge = edges[i];
    ofile << "  " << edge.first << " -> " << edge.second  << ";" << std::endl;
  }
  
  // close subgraph
  ofile << "}" << std::endl;

}



std::string TechTree::dump_node_label(const unsigned i) {

  std::stringstream out;
  out << "[";

  const Turk::TUnit & unit = tunits_[i];

  // UnitType
  if ( unit.getType() == Turk::WeaverTypes::Unit_t ) {
    const BWAPI::UnitType & bunit = unit.getUnit();
    out << "label=\"" << bunit.c_str() << "\" ";
    if ( bunit.isBuilding() ) out << "shape=\"box\" ";
    else out << "shape=\"triangle\" ";
  }
  // UpgradeType
  else if ( unit.getType() == Turk::WeaverTypes::Upgrade_t ) {
    const BWAPI::UpgradeType & bup = unit.getUpgrade();
    out << "label=\"" <<  bup.c_str() << " level: " << unit.getLevel() << "\" ";
    out << "shape=\"house\" ";
  }
  // Tech Type
  else if ( unit.getType() == Turk::WeaverTypes::Tech_t ) {
    const BWAPI::TechType & btech = unit.getTech();
    out << "label=\"" << btech.c_str() << "\" ";
    out << "shape=\"diamond\" ";
  }

  out << "]";

  return out.str();
}




void TechTree::dump_dot(const char * name ) {

  // open the file
  std::ofstream ofile(name,std::ofstream::out);

  // write the header information
  ofile << "digraph techtree {" << std::endl;

  // cycle over the races (make a subgraph for each race)
  for ( auto race: BWAPI::Races::allRaces() ) {

    if ( race.getID() != BWAPI::Races::Terran 
      && race.getID() != BWAPI::Races::Zerg
      && race.getID() != BWAPI::Races::Protoss ) continue;

    // construct sub graph
    dump_subgraph(ofile,race);

  }

  // close the graph
  ofile << "}" << std::endl;

  // close the file
  ofile.close();

}


/**
* Dump a generic csv file
*/
void dump_csv(const char *ofname,const int * data, const unsigned ncols, const unsigned nrows) {

}


int main(int argc, char **argv) {

  // parse command line arguments

  // tables:
  // myunitid (bit shifted int to contain upgrade options) to UnitType (static_cast of enum)
  // myunitid to upgrades
  // unit data
  // unit requirements
  // built from


  const unsigned n_units = BWAPI::UnitTypes::allUnitTypes().size();


  UnitCollection unit_collection;
  TechCollection tech_collection;
  UpgradeCollection upgrade_collection;
  TechTree tech_tree;


  // iterate over all units 
  for ( auto unit: BWAPI::UnitTypes::allUnitTypes() ) {

    // dump unit info
    unit_collection.push(unit);
    tech_tree.push(Turk::TUnit(unit));
      
  }

  // iterate over all tech types
  for ( auto tech: BWAPI::TechTypes::allTechTypes() ) {

    // dump tech info
    tech_collection.push(tech);
    tech_tree.push(Turk::TUnit(tech));

  }

  // iterate over all upgrade types
  for ( auto upgrade: BWAPI::UpgradeTypes::allUpgradeTypes() ) {

    // dump upgrade info 
    upgrade_collection.push(upgrade);
    tech_tree.push(Turk::TUnit(upgrade));
 
  }

  tech_tree.mkgraph();

  // dump the data
  unit_collection.dump_csv("unit_data.csv");
  tech_collection.dump_csv("tech_data.csv");
  upgrade_collection.dump_csv("upgrade_data.csv");
  tech_tree.dump_graph("tech_tree.csv");
  tech_tree.dump_map("full_tech_map.csv");
  tech_tree.dump_dot("tech_tree.pdf.gv");
  
  
}


