

#include "UnitManager.h"
#include "BaseManager.h"

#include "Logger.h"


using namespace Turk;

UnitManager Turk::umanity;


void UnitManager::process() {
	updateHUD();
} 

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
	
}


void UnitManager::onUnitCreate(BWAPI::Unit unit) {

	// if unit is a worker, assign to nearest base
	if (unit->getType().isWorker()) {
		// find nearest base
		for (unsigned i = 0; i != agent_count_; i++) {
			Turk::bot * b = agents_[i];
			if (b && b->type().compare("BaseManager") == 0 && unit->getDistance(b->location()) < 200 ) {
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

	// process the unit_queue_ 
	// transfer completed unit requests to the requesting bot.
	unsigned nq = unit_queue_.size();
	for (unsigned i = 0; i != nq; i++) {
		unit_request & rq = unit_queue_[i];
		if ( rq.unit_type == unit->getType() ) {
			unit_queue_.mask(i);
			std::vector<UnitProxy> ups(1, UnitProxy(unit));
			transfer(rq.producer, rq.requester, ups);
		}
	}
	
	// if the unit is a building
	if (unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self()) {
		// find the nearest base
		Turk::bot * bm = findNearestAgent(unit->getPosition(), "BaseManager");
		if (bm)
			dynamic_cast<BaseManager *>(bm)->buildingComplete(unit);
		
	}
	// if the unit is a combat unit
	else if (!unit->getType().isWorker() && !unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self()) {
		// assign to army
		Turk::bot * bm = findNearestAgent(unit->getPosition(), "ArmyManager");
		if (bm) {
			std::vector<UnitProxy> ulist(1);
			UnitProxy up(unit);
			ulist[0] = up;
			unit_map_.insert(std::pair<const bot *, UnitProxy>(bm, up));

			bm->addUnits(ulist);

			
		}
			
	}
}


void UnitManager::onUnitDestroy(BWAPI::Unit unit) {

	// lookup which agent owns the unit
	UnitProxy up(unit);
	const Turk::bot * b = unit_map_.rfind(up);

	// remove unit
	unit_map_.mask(unit);

	if(b)
	  const_cast<Turk::bot *>(b)->updateUnits();


}

void UnitManager::updateHUD() {
	HUD::Instance().clear(hud_lane_);

	// dump the registered agents
	HUD::Instance().write(hud_lane_, "Registered Agents");
	for (unsigned i = 0; i != agent_count_; i++) {

		if (!agents_[i]) continue;

		std::stringstream msg;
		msg << agents_[i]->name() << " (" << agents_[i]->location().x 
			<< "," << agents_[i]->location().y << ")";

		HUD::Instance().write(hud_lane_, msg.str().c_str());
	}

	// list the transfer request queue
	HUD::Instance().write(hud_lane_, "");
	HUD::Instance().write(hud_lane_, "Unit Request Queue");
	const unsigned nq = unit_queue_.nqueued();
	for (unsigned i = 0; i != nq; i++) {

		std::stringstream msg;
		msg << unit_queue_[i].producer->name() << "\n ->" << unit_queue_[i].requester->name()
			<< ":\n" << unit_queue_[i].unit_type;

		HUD::Instance().write(hud_lane_, msg.str().c_str());
	}
}
