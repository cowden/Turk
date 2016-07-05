#pragma once
#include "Common.h"

namespace Turk {

	// Commander Class
	class ScoutManager
	{
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

	private:

		// Scout Variables
		BWAPI::Unit m_Scouter;

	};

} // end Turk namespace
