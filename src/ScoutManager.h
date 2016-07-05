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
		void ScouterSaver(const BWAPI::Unit &);

		// Return a scout unit pointer
		const BWAPI::Unit & ScouterPresent();

		// Detect the enemy base
		bool EnemyFirstDetector(bool,const BWAPI::TilePosition &);

		static ScoutManager &Instance();

	private:

		// Scout Variables
		BWAPI::Unit m_Scouter;

	};

} // end Turk namespace
