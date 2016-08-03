#pragma once
#include "Common.h"


namespace Turk {

	// Commander Class
	class InformationManager
	{

	public:
		InformationManager();
		~InformationManager(){};

	private:
		// Scout Variables
		BWAPI::Position m_EnemyHomeBase;
	};

} // end Turk namespace