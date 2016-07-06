#pragma once

#include "Commander.h"
#include "Common.h"


using namespace Turk;

Commander::Commander()
	// : FirstScout(false)
{
	

}


//Commander::~Commander()
//{
//}

Commander & Commander::Instance()
{
	static Commander instance;
	return instance;
}



// Collect all valid units except dead bodies
void Commander::ValidUnitCollector(BWAPI::Unit ScouterUnit){

	int Pylon_Count = 0;
	int GateWay_Count = 0;
	int StarGate_Count = 0;
	int Nexus_Count = 0;
	int Probe_Count = 0;
	int Zealot_Count = 0;
	int Dragoon_Count = 0;
	int Corsair_Count = 0;

	_ValidUnits.clear();
	_BaseUnits.clear();
	_WorkerUnits.clear();

	for (auto &unit : BWAPI::Broodwar->self()->getUnits()){
		if (IsValidUnit(unit)){
			_ValidUnits.insert(unit);

			if (unit->getType() == BWAPI::UnitTypes::Protoss_Nexus){
				_BaseUnits.insert(unit);

				Nexus_Count = Nexus_Count + 1;
			}
			else if (unit->getType().isWorker()){
				_WorkerUnits.insert(unit);

				Probe_Count = Probe_Count + 1;
			}

			// Unit Counts
			if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot)
			{
				Zealot_Count = Zealot_Count + 1;
			}
			else if (unit->getType() == BWAPI::UnitTypes::Protoss_Dragoon)
			{
				Dragoon_Count = Dragoon_Count + 1;
			}
			else if (unit->getType() == BWAPI::UnitTypes::Protoss_Corsair)
			{
				Corsair_Count = Corsair_Count + 1;
			}
			else if (unit->getType() == BWAPI::UnitTypes::Protoss_Pylon)
			{
				Pylon_Count = Pylon_Count + 1;
			}
			else if (unit->getType() == BWAPI::UnitTypes::Protoss_Gateway)
			{
				GateWay_Count = GateWay_Count + 1;
			}
			else if (unit->getType() == BWAPI::UnitTypes::Protoss_Stargate){
				StarGate_Count = StarGate_Count + 1;
			}
		}
	}
	if (ScouterUnit){
		_WorkerUnits.erase(ScouterUnit);
	}

	UnitCount["Probe_Count"] = Probe_Count;
	UnitCount["Zealot_Count"] = Zealot_Count;
	UnitCount["Dragoon_Count"] = Dragoon_Count;
	UnitCount["Corsair_Count"] = Corsair_Count;

	UnitCount["Pylon_Count"] = Pylon_Count;
	UnitCount["GateWay_Count"] = GateWay_Count;
	UnitCount["StarGate_Count"] = StarGate_Count;
	UnitCount["Nexus_Count"] = Nexus_Count;

	// Future unit sets
	// SetScoutUnits();
	// SetBaseCombatUnits();
	// SetFieldCombatUnits();
}



std::map<std::string, int> Commander::UnitCounterPresenter(){
	return UnitCount;
}




BWAPI::Unitset Commander::MineralCollector(BWTA::BaseLocation * BasePoint){
	//MineralSets.clear();
	for (auto &mineral : BasePoint->getStaticMinerals()){
		MineralSets.insert(mineral);
	}		
	return MineralSets;
}

// BWAPI::Broodwar->sendText("%.2d %.2d / Unit: %s", targetBuildLocation.x, targetBuildLocation.y,	mineral->getType().c_str());


BWAPI::Unitset Commander::MineralPresent(){
	return MineralSets;
}


void Commander::MineralSaver(BWAPI::Unitset Mineral){
	MineralSets = Mineral;
}




void Commander::ScoutHander(BWAPI::Unit Scout){
	_WorkerUnits.erase(Scout);
}

// unit represent the point.
bool Commander::IsValidUnit(BWAPI::Unit unit){
	if (!unit){
		return false;
	}

	if (!unit->exists()){
		return false;
	}

	// Ignore the unit if it has one of the following status ailments
	if (unit->isLockedDown() || unit->isMaelstrommed() || unit->isStasised()){
		return false;
	}

	// Ignore the unit if it is in one of the following states
	if (unit->isLoaded() || !unit->isPowered() || unit->isStuck()){
		return false;
	}		

	// Ignore the unit if it is incomplete or busy constructing
	if (!unit->isCompleted() || unit->isConstructing()){
		return false;
	}
		
	// Find real units
	if (unit->isCompleted()
		&& unit->getHitPoints() > 0
		&& unit->exists()
		&& unit->getType() != BWAPI::UnitTypes::Unknown
		&& unit->getPosition().x != BWAPI::Positions::Unknown.x
		&& unit->getPosition().y != BWAPI::Positions::Unknown.y){
		return true;
	}
	else
	{
		return false;
	}

}





BWAPI::Unitset Commander::UnitSetPresent(){
	return _ValidUnits;
}


BWAPI::Unitset Commander::BasePresent(){
	return _BaseUnits;
}


BWAPI::Unitset Commander::WorkerPresent(){
	return _WorkerUnits;
}


void Commander::ProbeMaker(unsigned MaxWorkerCount){

	for (auto &unit : _BaseUnits){
		// Order the depot to construct more workers! But only when it is idle.
		if (unit->isIdle() && WorkerPresent().size() < MaxWorkerCount){
			// Train a worker
			unit->train(unit->getType().getRace().getWorker());
		}
	}
}

/*  Worker Tasks
1. Get Mineral
2. If it is too far from a base, come back and work.
3. Find the unoccupied closest mineral.
*/

void Commander::ProbeWork(int MaxMineralDist){
	for (auto &unit2 : _WorkerUnits){
		// Prepare Basic info: Location of the nearest base and its distance
		BWAPI::Unit ClosestBase = unit2->getClosestUnit(BWAPI::Filter::IsResourceDepot && 
														BWAPI::Filter::IsCompleted && 
														BWAPI::Filter::IsOwned);
		if (ClosestBase){
			double DistanceToBase = unit2->getDistance(ClosestBase);

			// if our worker is idle
			if (unit2->isIdle()) {
				// Order workers carrying a resource to return them to the center		     
				if (unit2->isCarryingGas() || unit2->isCarryingMinerals()){
					unit2->returnCargo();
				}

				// A worker is newly born.
				else if (unit2->isCompleted()){
					BWAPI::Unit ClosestMineral = nullptr;
					double closestDist = 100000;

					// For the given initial Minerals, find the closest mineral one by one		
					BWAPI::Unitset FirstMineralSet = MineralPresent();
					for (auto &mineral : FirstMineralSet){
						double dist = unit2->getDistance(mineral->getPosition());
						if (dist < closestDist){
							ClosestMineral = mineral;
							closestDist = dist;
						}
					}
					// Is it really closest? It is too far  or Mieral List is emphty !
					if (closestDist > MaxMineralDist){
						unit2->gather(ClosestBase->getClosestUnit(BWAPI::Filter::IsMineralField));
						BWTA::BaseLocation * Base = BWTA::getNearestBaseLocation(ClosestBase->getPosition());
						FirstMineralSet = MineralCollector(Base);
					}
					else{
						unit2->gather(ClosestMineral);
						FirstMineralSet.erase(ClosestMineral);
					}
					MineralSaver(FirstMineralSet);
				}

				// otherwise find a mineral patch to harvest.
				else{
					unit2->gather(ClosestBase->getClosestUnit(BWAPI::Filter::IsMineralField));
				}
			}

			// This code should be modified for future scouting action.
			// You are working at too far location.
			else if (unit2->isGatheringMinerals() || unit2->isGatheringGas()){
				if (DistanceToBase > MaxMineralDist){
					// Go back to your friends.
					unit2->gather(ClosestBase->getClosestUnit(BWAPI::Filter::IsMineralField));
				}
			}
		}
	}
}









