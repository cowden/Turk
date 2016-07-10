#pragma once

/////////////////////////////////////////
// HUD - Heads Up Display
// For the visualization of AI and Game information.
/////////////////////////////////////////

#include <BWAPI.h>

namespace Turk {

	class HUD {

	public:

		static HUD & Instance();

		void drawInterface();

	private:

		const std::vector<BWAPI::Unit> & getUnits();

	};
	 
} // end Turk namespace