#include "Scout.h"

using namespace Turk;

unsigned Scout::m_nScouts = 0U;


void Scout::updateHUD() {

	// clear the hud lane
	HUD::Instance().clear(hud_lane_);

	// write the current status
	std::stringstream state_stream;
	state_stream << "State: " << active_state_ << " " << isAlive_;
	HUD::Instance().write(hud_lane_, state_stream.str().c_str());

	state_stream.clear();
	state_stream.str(std::string());
	if (squads_.nheld() > 0) {
		state_stream << "Size: " << squads_[0]->size();
		HUD::Instance().write(hud_lane_, state_stream.str().c_str());
	}
}