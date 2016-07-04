#pragma once

#include "BuildingManager.h"
#include "Common.h"



BuildingManager::BuildingManager()
// : FirstScout(false)
{


}


//Commander::~Commander()
//{
//}

BuildingManager & BuildingManager::Instance()
{
	static BuildingManager instance;
	return instance;
}



void BuildingManager::MapConnector(){

	std::string Mapname = BWAPI::Broodwar->mapName().c_str();
	if (Mapname.compare("Python 1.3")){		// 
		PythonBuildingLocation();
	}
	
}



// int arr[3];


void BuildingManager::PythonBuildingLocation(){

	// Game Information <- This should be moved to the InformationManager.h
	BWAPI::Position homePosition = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
	BWAPI::Position HillPosition = BWTA::getNearestChokepoint(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()))->getCenter();

	// Tempral Variables
	BWAPI::TilePosition FirstPylonLocation;	
	BWAPI::TilePosition SecondPylonLocation;
	BWAPI::TilePosition TempTileLocation;
	BWAPI::TilePosition FirstGateWay;
	BWAPI::TilePosition FirstTech;
	

	if ((abs(homePosition.x - HillPosition.x) > abs(homePosition.y - HillPosition.y)) && (homePosition.x - HillPosition.x > 0)){   //   , Choke is left
		// Probably 12

		// Location of the first pylon		
		FirstPylonLocation.x = 76;
		FirstPylonLocation.y = 4;			
		PylonTilePosition.push_back(FirstPylonLocation);

		// Location of the Second pylon		
		SecondPylonLocation.x = 82;
		SecondPylonLocation.y = 12;
		PylonTilePosition.push_back(SecondPylonLocation);

		// Build the gateways left to the first pylon
		FirstGateWay.x = FirstPylonLocation.x - GateWay.tileWidth() * 2;
		FirstGateWay.y = FirstPylonLocation.y;
						
		// Build the upgrading points
		FirstTech.x = FirstPylonLocation.x + Pylon.tileWidth();
		FirstTech.y = FirstPylonLocation.y;
			
		// Update Building Positions
		BuildingManager::PylonPositioning(FirstPylonLocation,6);
		BuildingManager::GateWayPositioning(FirstGateWay,4);
		BuildingManager::TechBuildingPositioning(FirstTech,6);



		// Build the gateways left to the first pylon
		FirstGateWay.x = SecondPylonLocation.x - GateWay.tileWidth() * 2;
		FirstGateWay.y = SecondPylonLocation.y;

		// Build the upgrading pointsx
		FirstTech.x = SecondPylonLocation.x + Pylon.tileWidth();
		FirstTech.y = SecondPylonLocation.y;

		// Update Building Positions
		BuildingManager::PylonPositioning(SecondPylonLocation, 5);
		BuildingManager::GateWayPositioning(FirstGateWay,3);
		BuildingManager::TechBuildingPositioning(FirstTech,4);


		//  Choke Point
				
	}
	else if ((abs(homePosition.x - HillPosition.x) > abs(homePosition.y - HillPosition.y)) && (homePosition.x - HillPosition.x < 0)){  // Chock is East
		// Probably 6
		// BackYard 33 / 119

		// Location of the first pylon		
		FirstPylonLocation.x = 50;
		FirstPylonLocation.y = 115;
		PylonTilePosition.push_back(FirstPylonLocation);
		
		// Location of the second pylon		
		SecondPylonLocation.x = 38;
		SecondPylonLocation.y = 107;
		PylonTilePosition.push_back(SecondPylonLocation);
		
		// Build the gateways right to the first pylon
		FirstGateWay.x = FirstPylonLocation.x + Pylon.tileWidth();
		FirstGateWay.y = FirstPylonLocation.y;
				
		// Determine locations of gateways.
		// Build the upgrading points
		FirstTech.x = FirstPylonLocation.x - Forge.tileWidth();
		FirstTech.y = FirstPylonLocation.y;


		// Update Building Positions
		BuildingManager::PylonPositioning(FirstPylonLocation,6);
		BuildingManager::GateWayPositioning(FirstGateWay,4);
		BuildingManager::TechBuildingPositioning(FirstTech,6);

		
		// Build the gateways right to the first pylon
		FirstGateWay.x = SecondPylonLocation.x + Pylon.tileWidth();
		FirstGateWay.y = SecondPylonLocation.y;

		// Determine locations of gateways.
		// Build the upgrading points
		FirstTech.x = SecondPylonLocation.x - Forge.tileWidth();
		FirstTech.y = SecondPylonLocation.y;


		// Update Building Positions
		BuildingManager::PylonPositioning(SecondPylonLocation, 6);
		BuildingManager::GateWayPositioning(FirstGateWay,3);
		BuildingManager::TechBuildingPositioning(FirstTech,4);

		
	}
	else if ((abs(homePosition.x - HillPosition.x) < abs(homePosition.y - HillPosition.y)) && (homePosition.y - HillPosition.y > 0)){
		// Probably 9		
		// Location of the first pylon
		FirstPylonLocation.x = 15;   // Fix
		FirstPylonLocation.y = 77;
		PylonTilePosition.push_back(FirstPylonLocation);

		SecondPylonLocation.x = 12;   // Fix
		SecondPylonLocation.y = 89;
		PylonTilePosition.push_back(SecondPylonLocation);

		// Build the gateways right to the first pylon
		FirstGateWay.x = FirstPylonLocation.x + Pylon.tileWidth();
		FirstGateWay.y = FirstPylonLocation.y;

		// Determine locations of gateways.
		// Build the upgrading points
		FirstTech.x = FirstPylonLocation.x - Forge.tileWidth();
		FirstTech.y = FirstPylonLocation.y;

		// Update Building Positions
		BuildingManager::PylonPositioning(FirstPylonLocation,6);
		BuildingManager::GateWayPositioning(FirstGateWay,4);
		BuildingManager::TechBuildingPositioning(FirstTech,6);

		// choke 13 72


		// Build the gateways right to the first pylon
		FirstGateWay.x = SecondPylonLocation.x + Pylon.tileWidth();
		FirstGateWay.y = SecondPylonLocation.y;

		// Determine locations of gateways.
		// Build the upgrading points
		FirstTech.x = SecondPylonLocation.x - Forge.tileWidth();
		FirstTech.y = SecondPylonLocation.y;


		// Update Building Positions
		BuildingManager::PylonPositioning(SecondPylonLocation, 6);
		BuildingManager::GateWayPositioning(FirstGateWay,3);
		BuildingManager::TechBuildingPositioning(FirstTech,4);
				

	}
	else if ((abs(homePosition.x - HillPosition.x) < abs(homePosition.y - HillPosition.y)) && (homePosition.y - HillPosition.y < 0)){
		// Probably 3

		// Location of the first pylon
		FirstPylonLocation.x = 111;
		FirstPylonLocation.y = 43;
		PylonTilePosition.push_back(FirstPylonLocation);

		// Location of the second pylon
		SecondPylonLocation.x = 117;
		SecondPylonLocation.y = 26;
		PylonTilePosition.push_back(SecondPylonLocation);

		// Build the gateways left to the first pylon
		FirstGateWay.x = FirstPylonLocation.x - GateWay.tileWidth() * 2;
		FirstGateWay.y = FirstPylonLocation.y;

		// Build the upgrading points
		FirstTech.x = FirstPylonLocation.x + Pylon.tileWidth();
		FirstTech.y = FirstPylonLocation.y;

		// Update Building Positions
		BuildingManager::PylonPositioning(FirstPylonLocation,6);
		BuildingManager::GateWayPositioning(FirstGateWay,4);
		BuildingManager::TechBuildingPositioning(FirstTech,6);

		// Upgrade 117:32
		// choke 113 55	
		

		// Build the gateways left to the first pylon
		FirstGateWay.x = SecondPylonLocation.x - GateWay.tileWidth() * 2;
		FirstGateWay.y = SecondPylonLocation.y;

		// Build the upgrading points
		FirstTech.x = SecondPylonLocation.x - Pylon.tileWidth();
		FirstTech.y = SecondPylonLocation.y;

		// Update Building Positions
		BuildingManager::PylonPositioning(SecondPylonLocation, 6);
		BuildingManager::GateWayPositioning(FirstGateWay,3);
		BuildingManager::TechBuildingPositioning(FirstTech,4);

	}

	
	std::reverse(PylonTilePosition.begin(), PylonTilePosition.end());
	std::reverse(GateTilePosition.begin(), GateTilePosition.end());
	std::reverse(TechBuildingTileLocation.begin(), TechBuildingTileLocation.end());

}


void BuildingManager::ChokeDefensePylon(){

}


void BuildingManager::DefensePylonLocation(BWAPI::Unitset FirstMineralSet){
	
	// Build pylon close to the minerals
	BWAPI::Unit ClosestMineral = nullptr;
	BWAPI::Unit FarthestMineral = nullptr;
	double ClosestDist = 10000;
	double FarthestDist = 0;
	// For the given initial Minerals, find the closest mineral one by one
	for (auto &mineral : FirstMineralSet){
		BWAPI::Position MineralPosition = mineral->getPosition();
	
		double dist = MineralPosition.x + MineralPosition.y;
		if (dist < ClosestDist){
			ClosestMineral = mineral;
			ClosestDist = dist;
		}
		if (dist > FarthestDist){
			FarthestMineral = mineral;
			FarthestDist = dist;
		}
	}

	// Collect additional pylon location.
	BWAPI::TilePosition FirstMineralPosition = ClosestMineral->getTilePosition();
	BWAPI::TilePosition LastMineralPosition = FarthestMineral->getTilePosition();	

	FirstMineralPosition.x = FirstMineralPosition.x + 2;
	FirstMineralPosition.y = FirstMineralPosition.y - 2;
	LastMineralPosition.x = LastMineralPosition.x + 2;
	LastMineralPosition.y = LastMineralPosition.y + 2;

	std::vector<BWAPI::TilePosition> anothervector;
	anothervector.push_back(FirstMineralPosition);
	anothervector.push_back(LastMineralPosition);

	// 3rd and 4th pylons
	PylonTilePosition.insert(PylonTilePosition.begin() + 8, anothervector.begin(), anothervector.end());  	

}












// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Variable Handler
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
BWAPI::TilePosition BuildingManager::PylonLocationPresent(){
	if (!BWAPI::Broodwar->isBuildable(PylonTilePosition.back(), true)){
		PylonTilePosition.pop_back();
	}
	return PylonTilePosition.back();
}

void BuildingManager::PylonLocationRemover(){
	PylonTilePosition.pop_back();	
}

void BuildingManager::PylonLocationSaver(std::vector<BWAPI::TilePosition> TempPosition){
	PylonTilePosition = TempPosition;
}

BWAPI::TilePosition BuildingManager::GateWayLocationPresent(){
	if (!BWAPI::Broodwar->isBuildable(GateTilePosition.back(), true)){
		GateTilePosition.pop_back();
	}	
	return GateTilePosition.back();
}

void BuildingManager::GateWayLocationSaver(std::vector<BWAPI::TilePosition> TempPosition){
	GateTilePosition = TempPosition;
}

void BuildingManager::GateWayLocationRemover(){
	GateTilePosition.pop_back();
}



BWAPI::TilePosition BuildingManager::TechLocationPresent(){	
	if (!BWAPI::Broodwar->isBuildable(TechBuildingTileLocation.back(), true)){
		TechBuildingTileLocation.pop_back();
	}
	return TechBuildingTileLocation.back();
}

void BuildingManager::TechLocationSaver(std::vector<BWAPI::TilePosition> TempPosition){
	TechBuildingTileLocation = TempPosition;
}


void BuildingManager::TechLocationRemover(){
	TechBuildingTileLocation.pop_back();
}



BWAPI::TilePosition BuildingManager::GetExpansionBase(){

	BWAPI::TilePosition NexusLocation;
	
	BWAPI::TilePosition homeTilePosition = BWAPI::Broodwar->self()->getStartLocation();
	BWTA::BaseLocation * closestBase = nullptr;
	double minDistance = 100000;

	// for each base location
	for (BWTA::BaseLocation * base : BWTA::getBaseLocations()){
		BWAPI::TilePosition tile = base->getTilePosition();

		// // if the base has gas
		if (BWAPI::Broodwar->isBuildable(tile,true))
		{
			//  // get the tile position of the base
			double distanceFromHome = BWTA::getGroundDistance(homeTilePosition, tile);

			//  // the base's distance from our main nexus

			// if it is not connected, continue //island
			if (!BWTA::isConnected(homeTilePosition, tile)){
				continue;
			}

			if (!closestBase || distanceFromHome < minDistance)
			{
				closestBase = base;
				minDistance = distanceFromHome;
			}			
		}
	}
	return closestBase->getTilePosition();
}












// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Building Positioning 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void BuildingManager::PylonPositioning(BWAPI::TilePosition StartingPoint,int Numbers){		
	BWAPI::TilePosition TempTileLocation;		
	for (int a = 1; a < Numbers; a = a + 1){
		TempTileLocation.x = StartingPoint.x;
		TempTileLocation.y = StartingPoint.y + a * Pylon.tileHeight();
		PylonTilePosition.push_back(TempTileLocation);
	}
}


void BuildingManager::GateWayPositioning(BWAPI::TilePosition StartingPoint, int Numbers){	
	BWAPI::TilePosition TempTileLocation;
	
	for (int a = 0; a < Numbers; a = a + 1){
		TempTileLocation.x = StartingPoint.x;
		TempTileLocation.y = StartingPoint.y + a * GateWay.tileHeight();
		GateTilePosition.push_back(TempTileLocation); // Third Gateway

		TempTileLocation.x = StartingPoint.x + GateWay.tileWidth();
		TempTileLocation.y = StartingPoint.y + a * GateWay.tileHeight();
		GateTilePosition.push_back(TempTileLocation); // Fourth Gateway
	}
}


void BuildingManager::TechBuildingPositioning(BWAPI::TilePosition StartingPoint,int Numbers){
	BWAPI::TilePosition TempTileLocation;

	for (int a = 0; a < Numbers; a = a + 1){
		// Location of Cybernetics core
		TempTileLocation.x = StartingPoint.x;
		TempTileLocation.y = StartingPoint.y + a * Cybernetics.tileHeight();
		TechBuildingTileLocation.push_back(TempTileLocation);
	}

}


















// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Build Build Build
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//void BuildingManager::MrBuilder(BWAPI::Unit HeadQuater, BWAPI::UnitType BuildingTarget, BWAPI::TilePosition WishLocation){
//		
//	static int lastChecked = 0;
//	// If we are supply blocked and haven't tried constructing more recently
//	if (lastChecked + 200 < BWAPI::Broodwar->getFrameCount() && BWAPI::Broodwar->self()->incompleteUnitCount(BuildingTarget) == 0){
//		lastChecked = BWAPI::Broodwar->getFrameCount();
//
//		// Retrieve a unit that is capable of constructing the supply needed
//		BWAPI::Unit supplyBuilder = HeadQuater->getClosestUnit(BWAPI::Filter::GetType == BuildingTarget.whatBuilds().first && (BWAPI::Filter::IsIdle || BWAPI::Filter::IsGatheringMinerals));
//		// If a unit was found
//		if (supplyBuilder){			
//			BWAPI::TilePosition targetBuildLocation = BWAPI::Broodwar->getBuildLocation(BuildingTarget, WishLocation, 2);
//			if (targetBuildLocation){
//				if (!BWAPI::Broodwar->isExplored(targetBuildLocation)){
//					supplyBuilder->move(BWAPI::Position(targetBuildLocation));
//					//supplyBuilder->build(BuildingTarget, targetBuildLocation);
//				}
//				else{
//					// Order the builder to construct the supply structure				
//					supplyBuilder->build(BuildingTarget, targetBuildLocation);
//				}
//			}
//			else{
//				BWAPI::TilePosition targetBuildLocation = BWAPI::Broodwar->getBuildLocation(BuildingTarget, supplyBuilder->getTilePosition());
//				if (targetBuildLocation){
//					// Order the builder to construct the supply structure				
//					if (!BWAPI::Broodwar->isExplored(targetBuildLocation)){
//						supplyBuilder->move(BWAPI::Position(targetBuildLocation));
//					}
//					else{
//						// Order the builder to construct the supply structure				
//						supplyBuilder->build(BuildingTarget, targetBuildLocation);
//					}
//				}
//			}			
//		}
//	}
//}

// isExplored

void BuildingManager::MrBuilder(BWAPI::Unit HeadQuater, BWAPI::UnitType BuildingTarget, BWAPI::TilePosition WishLocation){

	static int lastChecked = 0;
	// If we are supply blocked and haven't tried constructing more recently
	if (lastChecked + 200 < BWAPI::Broodwar->getFrameCount() && BWAPI::Broodwar->self()->incompleteUnitCount(BuildingTarget) == 0){
		lastChecked = BWAPI::Broodwar->getFrameCount();

		// Retrieve a unit that is capable of constructing the supply needed
		BWAPI::Unit supplyBuilder = HeadQuater->getClosestUnit(BWAPI::Filter::GetType == BuildingTarget.whatBuilds().first && (BWAPI::Filter::IsIdle || BWAPI::Filter::IsGatheringMinerals));
		// If a unit was found
		if (supplyBuilder){

			BWAPI::TilePosition targetBuildLocation;
			if (WishLocation){ 
				targetBuildLocation = BWAPI::Broodwar->getBuildLocation(BuildingTarget, WishLocation, 2); 
			}
			else{
				targetBuildLocation = BWAPI::Broodwar->getBuildLocation(BuildingTarget, supplyBuilder->getTilePosition());
			}			

			if (targetBuildLocation){
				// Order the builder to construct the supply structure				
				supplyBuilder->build(BuildingTarget, targetBuildLocation);
			}
			else{
				BWAPI::TilePosition targetBuildLocation = BWAPI::Broodwar->getBuildLocation(BuildingTarget, supplyBuilder->getTilePosition());
				if (targetBuildLocation){
					// Order the builder to construct the supply structure				
					supplyBuilder->build(BuildingTarget, targetBuildLocation);
				}
			}
		}
	}
}