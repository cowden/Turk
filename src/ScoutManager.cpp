#include "ScoutManager.h"


using namespace Turk;


ScoutManager::ScoutManager()
	:Scouter(nullptr)
{


}


ScoutManager & ScoutManager::Instance()
{
	static ScoutManager instance;
	return instance;
}







// Scout Related Functions

// Save Scouter Pointer
void ScoutManager::ScouterSaver(BWAPI::Unit ScouterUnit){
	Scouter = ScouterUnit;
}




// Return Scouter Pointer
BWAPI::Unit ScoutManager::ScouterPresent(){
	return Scouter;
}



bool ScoutManager::EnemyFirstDetector(bool Detector, BWAPI::TilePosition Expansion){
	
	BWAPI::TilePosition Right;
	Right.x = Expansion.x + 4;
	Right.y = Expansion.y + 4;

	if (!BWAPI::Broodwar->isExplored(Expansion) || !BWAPI::Broodwar->isExplored(Right)){
		//BWAPI::Broodwar->sendText("Hard Way %.2d %.2d ", Expansion.x, Expansion.y);
		Scouter->move(BWAPI::Position(Expansion));
		return Detector;
	}


	// First Detect the enemy base	
	// Collect enemy building information		
	for (auto & unit : BWAPI::Broodwar->enemy()->getUnits()){
		if (unit->getType().isBuilding()){
			Detector = true;
			break;
		}
	}

	if (!Detector){
		for (BWTA::BaseLocation * startLocation : BWTA::getStartLocations()){
			// if we haven't explored it yet
			if (!BWAPI::Broodwar->isExplored(startLocation->getTilePosition())){
				BWAPI::Position targetPosition = BWAPI::Position(startLocation->getTilePosition());
				Scouter->move(targetPosition);
			}
		}
	}
	

	return Detector;
}