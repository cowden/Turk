#pragma once
#include "Common.h"



// Commander Class
class ScoutManager
{

	// Scout Variables
	BWAPI::Unit Scouter;

	

	
	

public:
	ScoutManager();
	~ScoutManager(){};




	// Save Scouter pointer
	void ScouterSaver(BWAPI::Unit);

	// Return a scout unit pointer
	BWAPI::Unit ScouterPresent();


	// Detect the enemy base
	bool EnemyFirstDetector(bool, BWAPI::TilePosition);



	static ScoutManager &Instance();


};

