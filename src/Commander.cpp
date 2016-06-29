#include "Commander.h"
#include <BWAPI.h>
#include <BWTA.h>



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
void Commander::ValidUnitCollector(){
	_ValidUnits.clear();
	SetValidUnits();

	// Future unit sets
	// SetScoutUnits();
	// SetBaseCombatUnits();
	// SetFieldCombatUnits();
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




// Make a set of valid units
void Commander::SetValidUnits(){
	for (auto &unit : BWAPI::Broodwar->self()->getUnits()){
		if (IsValidUnit(unit)){
			_ValidUnits.insert(unit);

			if (unit->getType().isResourceDepot()){
				_BaseUnits.insert(unit);
			}
			else if (unit->getType().isWorker()){
				_WorkerUnits.insert(unit);
			}
		}
	}	
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








std::map<std::string, int> Commander::UnitCounter(){

	int Pylon_Count = 0;
	int GateWay_Count = 0;
	int Nexus_Count = 0;
	int Probe_Count = 0;
	int Zealot_Count = 0;
	int Dragoon_Count = 0;
	int Corsair_Count = 0;

	for (auto & unit : _ValidUnits)
	{
		// If the unit is a worker unit
		if (unit->getType().isWorker())
		{
			Probe_Count = Probe_Count + 1;
		}
		else if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot)
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
		else if (unit->getType() == BWAPI::UnitTypes::Protoss_Nexus)
		{
			Nexus_Count = Nexus_Count + 1;
		}
	}

	UnitCount["Probe_Count"] = Probe_Count;
	UnitCount["Zealot_Count"] = Zealot_Count;
	UnitCount["Corsair_Count"] = Corsair_Count;
	
	UnitCount["Pylon_Count"] = Pylon_Count;
	UnitCount["GateWay_Count"] = GateWay_Count;
	UnitCount["Nexus_Count"] = Nexus_Count;

	return UnitCount;

}