#pragma once

#include "BuildingManager.h"
#include "Common.h"


using namespace Turk;

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
		FirstPylonLocation.x = 74;
		FirstPylonLocation.y = 2;			
		PylonTilePosition.push_back(FirstPylonLocation);

		TempTileLocation.x = FirstPylonLocation.x + Pylon.tileWidth();
		TempTileLocation.y = FirstPylonLocation.y;
		PylonTilePosition.push_back(TempTileLocation);

		// Location of the Second pylon		
		SecondPylonLocation.x = 84;
		SecondPylonLocation.y = 12;
		PylonTilePosition.push_back(SecondPylonLocation);

		TempTileLocation.x = SecondPylonLocation.x + Pylon.tileWidth();
		TempTileLocation.y = SecondPylonLocation.y;
		PylonTilePosition.push_back(TempTileLocation);

		// Build the gateways left to the first pylon
		FirstGateWay.x = FirstPylonLocation.x - GateWay.tileWidth() - 1;
		FirstGateWay.y = FirstPylonLocation.y;
						
		// Build the upgrading points
		FirstTech.x = FirstPylonLocation.x + 2 * Pylon.tileWidth();
		FirstTech.y = FirstPylonLocation.y;
			
		// Update Building Positions
		BuildingManager::PylonPositioning(FirstPylonLocation,6);
		BuildingManager::GateWayPositioning(FirstGateWay,4);
		BuildingManager::TechBuildingPositioning(FirstTech,5);



		// Build the gateways left to the first pylon
		FirstGateWay.x = SecondPylonLocation.x - GateWay.tileWidth() - 1;
		FirstGateWay.y = SecondPylonLocation.y;

		// Build the upgrading pointsx
		FirstTech.x = SecondPylonLocation.x + 2 * Pylon.tileWidth();
		FirstTech.y = SecondPylonLocation.y;

		// Update Building Positions
		BuildingManager::PylonPositioning(SecondPylonLocation, 4);
		BuildingManager::GateWayPositioning(FirstGateWay,3);
		BuildingManager::TechBuildingPositioning(FirstTech,4);


		//  Choke Point
				
	}
	else if ((abs(homePosition.x - HillPosition.x) > abs(homePosition.y - HillPosition.y)) && (homePosition.x - HillPosition.x < 0)){  // Chock is East
		// Probably 6
		// BackYard 33 / 119

		// Location of the first pylon		
		FirstPylonLocation.x = 49;
		FirstPylonLocation.y = 110;
		PylonTilePosition.push_back(FirstPylonLocation);
		
		TempTileLocation.x = FirstPylonLocation.x + Pylon.tileWidth();
		TempTileLocation.y = FirstPylonLocation.y;
		PylonTilePosition.push_back(TempTileLocation);

		// Location of the second pylon		
		SecondPylonLocation.x = 34;
		SecondPylonLocation.y = 107;
		PylonTilePosition.push_back(SecondPylonLocation);


		TempTileLocation.x = SecondPylonLocation.x + Pylon.tileWidth();
		TempTileLocation.y = SecondPylonLocation.y;
		PylonTilePosition.push_back(TempTileLocation);
		
		// Build the gateways right to the first pylon
		FirstGateWay.x = FirstPylonLocation.x + 2* Pylon.tileWidth()+1;
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
		FirstGateWay.x = SecondPylonLocation.x + 2 * Pylon.tileWidth() + 1;
		FirstGateWay.y = SecondPylonLocation.y;

		// Determine locations of gateways.
		// Build the upgrading points
		FirstTech.x = SecondPylonLocation.x - Forge.tileWidth();
		FirstTech.y = SecondPylonLocation.y;


		// Update Building Positions
		BuildingManager::PylonPositioning(SecondPylonLocation, 4);
		BuildingManager::GateWayPositioning(FirstGateWay,3);
		BuildingManager::TechBuildingPositioning(FirstTech,4);

		
	}
	else if ((abs(homePosition.x - HillPosition.x) < abs(homePosition.y - HillPosition.y)) && (homePosition.y - HillPosition.y > 0)){
		// Probably 9		
		// Location of the first pylon
		FirstPylonLocation.x = 18;   // Fix
		FirstPylonLocation.y = 73;
		PylonTilePosition.push_back(FirstPylonLocation);

		TempTileLocation.x = FirstPylonLocation.x + Pylon.tileWidth();
		TempTileLocation.y = FirstPylonLocation.y;
		PylonTilePosition.push_back(TempTileLocation);

		SecondPylonLocation.x = 12;   // Fix
		SecondPylonLocation.y = 89;
		PylonTilePosition.push_back(SecondPylonLocation);

		TempTileLocation.x = SecondPylonLocation.x + Pylon.tileWidth();
		TempTileLocation.y = SecondPylonLocation.y;
		PylonTilePosition.push_back(TempTileLocation);

		// Build the gateways right to the first pylon
		// Build the gateways left to the first pylon		
		FirstGateWay.x = FirstPylonLocation.x - GateWay.tileWidth() - 1;
		FirstGateWay.y = FirstPylonLocation.y;

		// Build the upgrading points
		FirstTech.x = FirstPylonLocation.x + 2 * Pylon.tileWidth();
		FirstTech.y = FirstPylonLocation.y;

		// Update Building Positions
		BuildingManager::PylonPositioning(FirstPylonLocation,5);
		BuildingManager::GateWayPositioning(FirstGateWay,3);
		BuildingManager::TechBuildingPositioning(FirstTech,5);

		// choke 13 72


		// Build the gateways right to the first pylon
		FirstGateWay.x = SecondPylonLocation.x - GateWay.tileWidth() - 1;
		FirstGateWay.y = SecondPylonLocation.y;

		// Build the upgrading pointsx
		FirstTech.x = SecondPylonLocation.x + 2 * Pylon.tileWidth();
		FirstTech.y = SecondPylonLocation.y;


		// Update Building Positions
		BuildingManager::PylonPositioning(SecondPylonLocation, 4);
		BuildingManager::GateWayPositioning(FirstGateWay,3);
		BuildingManager::TechBuildingPositioning(FirstTech,4);
				

	}
	else if ((abs(homePosition.x - HillPosition.x) < abs(homePosition.y - HillPosition.y)) && (homePosition.y - HillPosition.y < 0)){
		// Probably 3

		// Location of the first pylon
		FirstPylonLocation.x = 109;
		FirstPylonLocation.y = 44;
		PylonTilePosition.push_back(FirstPylonLocation);

		TempTileLocation.x = FirstPylonLocation.x + Pylon.tileWidth();
		TempTileLocation.y = FirstPylonLocation.y;
		PylonTilePosition.push_back(TempTileLocation);

		// Location of the second pylon
		SecondPylonLocation.x = 117;
		SecondPylonLocation.y = 26;
		PylonTilePosition.push_back(SecondPylonLocation);

		TempTileLocation.x = SecondPylonLocation.x + Pylon.tileWidth();
		TempTileLocation.y = SecondPylonLocation.y;
		PylonTilePosition.push_back(TempTileLocation);

		// Build the gateways left to the first pylon		
		FirstGateWay.x = FirstPylonLocation.x - GateWay.tileWidth() - 1;
		FirstGateWay.y = FirstPylonLocation.y;

		// Build the upgrading points
		FirstTech.x = FirstPylonLocation.x + 2 * Pylon.tileWidth() ;
		FirstTech.y = FirstPylonLocation.y;

		// Update Building Positions
		BuildingManager::PylonPositioning(FirstPylonLocation,5);
		BuildingManager::GateWayPositioning(FirstGateWay,3);
		BuildingManager::TechBuildingPositioning(FirstTech,5);

		// Upgrade 117:32
		// choke 113 55	
		

		// Build the gateways left to the first pylon
		FirstGateWay.x = SecondPylonLocation.x - GateWay.tileWidth() - 1;
		FirstGateWay.y = SecondPylonLocation.y;

		// Build the upgrading pointsx
		FirstTech.x = SecondPylonLocation.x + 2 * Pylon.tileWidth();
		FirstTech.y = SecondPylonLocation.y;

		// Update Building Positions
		BuildingManager::PylonPositioning(SecondPylonLocation, 4);
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
	if (!BWAPI::Broodwar->canBuildHere(PylonTilePosition.back(), Pylon)){
		//BWAPI::Broodwar->sendText("I cannot build a pylon here");
		PylonTilePosition.pop_back();
	}
	return PylonTilePosition.back();
}

std::vector<BWAPI::TilePosition> BuildingManager::PylonSetPresent(){
	return PylonTilePosition;
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



void BuildingManager::GetExpansionBase(BWAPI::TilePosition EnemyTilePosition, BWAPI::TilePosition HomeTilePosition){
	

	std::map<double, BWAPI::TilePosition> NexusLoc;
	std::map<double, BWAPI::TilePosition>::iterator i;

	// This is only for nexus
	BWAPI::UnitType BuildingTarget = Nexus;
	
	BWTA::BaseLocation * closestBase = nullptr;
	double minDistance = 100000;

	// for each base location
	for (BWTA::BaseLocation * base : BWTA::getBaseLocations()){
		BWAPI::TilePosition tile = base->getTilePosition();

		// // if the base has gas
		if (BWAPI::Broodwar->isBuildable(tile, true)){
			//  // get the tile position of the base
			double distanceFromHome = BWTA::getGroundDistance2(HomeTilePosition, tile) - BWTA::getGroundDistance2(EnemyTilePosition, tile);

			// if it is not connected, continue //island
			if (!BWTA::isConnected(HomeTilePosition, tile)){
				continue;
			}

			NexusLoc[distanceFromHome] = tile;
		}
	}

	// Move expansion location in the map to the vector
	for (i = NexusLoc.begin(); i != NexusLoc.end(); i++){		
		ExpansionLocation.push_back(i->second);
	}	
	
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

		TempTileLocation.x = StartingPoint.x  + Pylon.tileWidth();
		TempTileLocation.y = StartingPoint.y  + a * Pylon.tileHeight();
		PylonTilePosition.push_back(TempTileLocation);

	}
}


void BuildingManager::GateWayPositioning(BWAPI::TilePosition StartingPoint, int Numbers){	
	BWAPI::TilePosition TempTileLocation;
	
	for (int a = 0; a < Numbers; a = a + 1){
		TempTileLocation.x = StartingPoint.x;
		TempTileLocation.y = StartingPoint.y + a * (GateWay.tileHeight());
		GateTilePosition.push_back(TempTileLocation); // Third Gateway

	}
}


void BuildingManager::TechBuildingPositioning(BWAPI::TilePosition StartingPoint,int Numbers){
	BWAPI::TilePosition TempTileLocation;

	for (int a = 0; a < Numbers; a = a + 1){
		// Location of Cybernetics core
		TempTileLocation.x = StartingPoint.x;
		TempTileLocation.y = StartingPoint.y + a * (Cybernetics.tileHeight());
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

void BuildingManager::BuildingFunction(BWAPI::Unit HeadQuater, BWAPI::UnitType BuildingTarget){
	
	
		

	static int lastChecked = 0;
	// If we are supply blocked and haven't tried constructing more recently
	if (lastChecked + 100 < BWAPI::Broodwar->getFrameCount() && BWAPI::Broodwar->self()->incompleteUnitCount(BuildingTarget) == 0){
		lastChecked = BWAPI::Broodwar->getFrameCount();

		// Get Mr. Builder.
		if (!MrBuilder){
			MrBuilder = HeadQuater->getClosestUnit(BWAPI::Filter::IsWorker && (BWAPI::Filter::IsGatheringMinerals || BWAPI::Filter::IsCarryingMinerals),99999);
		}

		// Determine the building locations
		std::vector<BWAPI::TilePosition>	Position;
		if (BuildingTarget == Pylon){
			Position = PylonTilePosition;
		}
		else if (BuildingTarget == GateWay || BuildingTarget == Stargate){
			Position = GateTilePosition;
		}
		else if (BuildingTarget == Nexus){
			ExpansionLocation;		
		}
		else{			
			Position = TechBuildingTileLocation;
		}


		// Determine the Building Location 
		BWAPI::TilePosition targetBuildLocation=BWAPI::TilePositions::None;
	
		// canBuildHere(Position.back(), BuildingTarget) isBuildable(Position.back(), true) 
		//// Can I build here?
		while (!Position.empty()){
			if (BWAPI::Broodwar->canBuildHere(Position.back(), BuildingTarget)){
				break;
			}
			else{
				BWAPI::Broodwar->sendText("I cannot build here %.2d : %.2d", Position.back().x, Position.back().y);
				Position.pop_back();
			}
		}

		// There is no possible building location
		if (Position.empty()){
			targetBuildLocation = BWAPI::Broodwar->getBuildLocation(BuildingTarget, HeadQuater->getTilePosition(), 32);
			BWAPI::Broodwar->sendText("Location vector is empty %.2d : %.2d", targetBuildLocation.x, targetBuildLocation.y);
		}
		// Or we can build it here !!!
		else{
			targetBuildLocation = Position.back(); // BWAPI::Broodwar->getBuildLocation(BuildingTarget, Position.back(), 4); //  
			// In worst case
			if (!targetBuildLocation){
				targetBuildLocation = BWAPI::Broodwar->getBuildLocation(BuildingTarget, HeadQuater->getTilePosition(), 32);
			}
		}
	
		
		


		// If a builder and location are secured
		if (MrBuilder && targetBuildLocation){

			if (!BWAPI::Broodwar->isExplored(targetBuildLocation) && BuildingTarget.mineralPrice()*0.8 < BWAPI::Broodwar->self()->minerals()){
				MrBuilder->move(BWAPI::Position(targetBuildLocation));				
			}
			else{
				MrBuilder->build(BuildingTarget, targetBuildLocation);				
			}				
		}

		//// Return the Variables
		//if (BuildingTarget == Pylon){
		//	PylonTilePosition = Position;
		//}
		//else if (BuildingTarget == GateWay || BuildingTarget == Stargate){
		//	GateTilePosition = Position;
		//}
		//else if (BuildingTarget == Nexus){
		//	ExpansionLocation = Position;
		//}
		//else{
		//	TechBuildingTileLocation = Position;
		//}


	}
}

void BuildingManager::MrBuilderRemover(){
	MrBuilder = nullptr;
}



//BWAPI::Broodwar->sendText("Location Existing %.2d : %.2d", WishLocation.x, WishLocation.y);
//BWAPI::Broodwar->sendText("No Location %.2d : %.2d", WishLocation.x, WishLocation.y);