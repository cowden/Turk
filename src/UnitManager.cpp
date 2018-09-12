

#include "UnitManager.h"
#include "BaseManager.h"

#include "Logger.h"


using namespace Turk;

UnitManager Turk::umanity;


void UnitManager::process() {} 

void UnitManager::process_request() {}

void UnitManager::update_unit_maps() {}

void UnitManager::initialize() {
	// assign initial workers to registered BaseManager

	const bot * bm = NULL;
	const bot * am = NULL;
	for (unsigned i = 0; i != agent_count_; i++) {
		const bot * a = agents_[i];
		if (a == NULL) continue;
		const BaseManager * bm2 = dynamic_cast<const BaseManager*>(a);
		std::stringstream msg;
		msg << "found agent [" << i << "] " <<  a->type() << " at: 0x" << std::hex << (int)a;
		Turk::Logger::instance()->log(m_name.c_str(), msg.str().c_str());
		
		if (a->type().compare("BaseManager") == 0) bm = a;
		else if (a->type().compare("ArmyManager") == 0) am = a;
	}
	
	// get a list of all units
    // assign each worker unit to the BaseManager
	// assign all combat units to the ArmyManager (in case such units exist).
	for (auto &u : BWAPI::Broodwar->self()->getUnits()) {
		if (u->getType().isWorker() && bm != NULL ) {
			std::string msg("loading worker to ");
			msg += bm->name();
			Turk::Logger::instance()->log(m_name.c_str(), msg.c_str());
			//unit_map_.insert(std::pair<bot *, BWAPI::Unit>(bm, u));
		}
	}
}


void UnitManager::onUnitCreate(BWAPI::Unit unit) {

	// if unit is a worker, assign to nearest base
	if (unit->getType().isWorker()) {
		// find nearest base
		for (unsigned i = 0; i != agent_count_; i++) {
			Turk::bot * b = agents_[i];
			if (b->type().compare("BaseManager") == 0 && unit->getDistance(b->location()) < 200 ) {
				// assign worker to base
				std::vector<UnitProxy> ulist(1);
                                UnitProxy up(unit);
				ulist[0] = up;
				b->addUnits(ulist);
				unit_map_.insert(std::pair<const bot *, UnitProxy>(b, up));
			}
		}
	}
	else if (unit->getType().isBuilding()) {
		// find nearest base
		Turk::bot * b = findNearestAgent(unit->getPosition(),"BaseManager");
		if (b)
			dynamic_cast<BaseManager *>(b)->buildingConstruction(unit);
	}
	else {
		// log an error, but keep playing
	}
}

void UnitManager::onUnitComplete(BWAPI::Unit unit) {
	
	// if the unit is a building
	if (unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self()) {
		// find the nearest base
		Turk::bot * bm = findNearestAgent(unit->getPosition(), "BaseManager");
		if (bm)
			dynamic_cast<BaseManager *>(bm)->buildingComplete(unit);
		
	}
}
