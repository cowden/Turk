 
#include "BaseManager.h"

#include "Logger.h"


using namespace Turk;

unsigned BaseManager::m_nBases = 0U;

void BaseManager::loadModel(const model_args & args) {

	// log the model load 
	std::string msg;
	msg = "Loading model";
	Turk::Logger::instance()->log(m_name.c_str(),msg.c_str());

	// set this bot's race (load specific techtree)
	race_ = args.trace_;

	// find the original resource depot
	for (auto b : BWAPI::Broodwar->self()->getUnits()) {
		if (b->getType().isResourceDepot()) {
			depot_ = b;
			//BWAPI::Position pos = depot_->getPosition();
			loc_ = depot_->getPosition();
		}
	}


	// map race's units

	// get initial workers and set to collecting minerals
	workers_ = umanity.getUnits(this);
	for ( unsigned i=0; i != workers_.size(); i++ ){
		BWAPI::Unit w = workers_[i];
		if (!w->gather(w->getClosestUnit(BWAPI::Filter::IsMineralField))) {
			BWAPI::Broodwar << BWAPI::Broodwar->getLastError() << std::endl;
		}
	}

	// load the initial build order
	// get the path from configuration
	std::stringstream build_name;
	//build_name << std::getenv("TURKDIR") << "\\data\\Terran\\BaseManager\\Terran_twofactoryvulture_v1.txt";
	//build_name << "C:\\\\Users\\User\\Desktop\\SCAI\\Turk\\data\\Terran\\BaseManager\\Terran_twofactoryvulture_v1.txt";
	build_name << "C:\\Users\\User\\Desktop\\SCAI\\Turk\\data\\Terran\\BaseManager" << "\\Terran_twobarracks_v1.txt";
	initialize_build_queue(build_name.str());

}


void BaseManager::initialize_build_queue(const std::string & name) {
	// log the model load
	std::stringstream msg;
	msg << "Loading build order " << name;
	Turk::Logger::instance()->log(m_name.c_str(), msg.str().c_str());

	msg.clear();
	msg.str(std::string());

	// open the file
	std::ifstream in(name.c_str(),std::ifstream::in);
	int err = errno;

	if (!in.is_open()) {
	
		msg << "There was an error loading the build order! " << err << " " << std::strerror(err);
		Turk::Logger::instance()->log(m_name.c_str(), msg.str().c_str());

		in.close();
		return;
	}
	
	char txt[500];
	in.getline(txt, 500);
	while (!in.eof() && in.good() && in.is_open()) {
		
		

		// parse the line
		std::stringstream ss(txt);
		std::string supply_text;
		std::string theUnit;

		ss >> supply_text;
		ss >> theUnit;

		int loc = supply_text.find('/');

		unsigned supply = atoi(supply_text.substr(0, loc).c_str());
		unsigned base_supply = atoi(supply_text.substr(loc + 1).c_str());

		// find the TUnit that matches theUnit.
		TUnit unit = weaver.getUnitType(theUnit);

		build_queue_.push(build_order_struct(supply, base_supply, unit));

		in.getline(txt, 500, '\n');

	}

	in.close();

}


void BaseManager::building(BWAPI::UnitType bu) {
	// find a worker
	// grab one going to the mineral field
	BWAPI::Unit builder;
	for (auto w : workers_) {
		if (w->isIdle()) {
			builder = w;
			break;
		} else if (w->getTarget() && w->getTarget()->getType().isMineralField() && !w->isCarryingMinerals()) {
			builder = w;
			break;
		}
			
	}

	// find a location and build it
	bool searching = true;
	while (searching) {

		// if the building is a refinery
		if (bu.isRefinery()) {
			// find the nearest gas to the base location
			//BWAPI::Position pos(findGeyser());
			BWAPI::TilePosition tp(findGeyser());
			BWAPI::TilePosition buildPosition = BWAPI::Broodwar->getBuildLocation(bu, tp);
			std::stringstream msg;
			msg << "Attempting to build Refinery @ " << buildPosition;
			BWAPI::Broodwar << msg.str() << std::endl;
			Logger::instance()->log(name().c_str(), msg.str().c_str());
			bool success = builder->build(bu, buildPosition);
			searching = !success;
			continue;
		}

		BWAPI::TilePosition delta(rand() % (8*200 + 1) - 100, rand() % (8*200 + 1) - 100);
		BWAPI::TilePosition pos = BWAPI::Broodwar->getBuildLocation(bu,depot_->getTilePosition() + delta,500);
		std::stringstream msg;
		msg << "Searching to build " << bu.getName() << " @ " << pos;
		BWAPI::Broodwar << msg.str() << std::endl;
		Logger::instance()->log(name().c_str(), msg.str().c_str());
		bool success = builder->build(bu, pos);

		searching = !success;
		//searching = false;
	}

	
}

void BaseManager::updateHUD() {
	
	HUD::Instance().clear(hud_lane_);

	// dump the build queue
	const unsigned n = build_queue_.nqueued();
	for (unsigned i = 0; i != n; i++) {
		const TUnit & bu = build_queue_[i].unit;
		if (bu.getType() == WeaverTypes::Unit_t)
			HUD::Instance().write(hud_lane_, bu.getUnit().getName());
	}
}


BWAPI::TilePosition BaseManager::findGeyser() {
	
	double dist = INFINITY;
	BWAPI::TilePosition nearest;

	// cycle over all geysers
	for (auto geyser : BWAPI::Broodwar->getStaticGeysers()) {
		if (geyser->getType() != BWAPI::UnitTypes::Resource_Vespene_Geyser)
			continue;
		double gd = depot_->getDistance(geyser->getInitialPosition());
		if (gd < dist) {
			dist = gd;
			nearest = geyser->getInitialTilePosition();
		}
	}

	std::stringstream msg;
	msg << "Found geyser @ " << nearest;
	Logger::instance()->log(name().c_str(), msg.str().c_str());
	BWAPI::Broodwar << msg.str() << std::endl;

	return nearest;
}