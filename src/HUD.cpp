
#include "HUD.h"


using namespace Turk;
using namespace BWAPI;


HUD & HUD::Instance(){
	static HUD instance;
	return instance;
}

void HUD::drawInterface(){

	// geography
	// draw choke point location
	const std::vector<unsigned> & chks = artie.get_chokes();
	const unsigned N = chks.size();
	for (unsigned i = 1; i != N; i++ ) {
		if (chks[i]) {
			
			const Turk::region & reg = artie[i];
			const unsigned x = reg.position().x;
			const unsigned y = reg.position().y;
			if ( reg.depth() < 15  ) BWAPI::Broodwar->drawCircleMap(BWAPI::Position(BWAPI::WalkPosition(x,y)),50,BWAPI::Colors::Red);

		}
	}


	// training queue
	std::vector < std::pair<BWAPI::UnitType, int> > trainingQueue;

	// draw unit information
	for (auto unit : BWAPI::Broodwar->getAllUnits()){
		const BWAPI::Position & pos = unit->getPosition();

		const BWAPI::UnitType & ut = unit->getType();
		int left = pos.x - ut.dimensionLeft();
		int right = pos.x + ut.dimensionRight();
		int top = pos.y - ut.dimensionUp();
		int bottom = pos.y + ut.dimensionDown();

		// draw unit health
		if (!ut.isResourceContainer() && ut.maxHitPoints() > 0) {
			double hp = (double)unit->getHitPoints() / ut.maxHitPoints();

			BWAPI::Broodwar->drawBoxMap(BWAPI::Position(left, top - 10), BWAPI::Position(right, top - 6), BWAPI::Colors::Grey, true);
			BWAPI::Broodwar->drawBoxMap(BWAPI::Position(left, top - 10), BWAPI::Position(left + (int)((right - left)*hp),top-6), BWAPI::Colors::Green, true);
		}

		// draw shield capacity
		if (!ut.isResourceContainer() && ut.maxShields() > 0) {
			double sh = (double)unit->getShields() / ut.maxShields();

			BWAPI::Broodwar->drawBoxMap(BWAPI::Position(left, top - 14), BWAPI::Position(right, top - 10), BWAPI::Colors::Grey, true);
			BWAPI::Broodwar->drawBoxMap(BWAPI::Position(left, top - 14), BWAPI::Position(left + (int)((right - left)*sh), top - 10), BWAPI::Colors::Blue, true);
		}

		// fill the training queue
		if (unit->getRemainingTrainTime() > 0) {
			int count = 0;
			for (auto tunit : unit->getTrainingQueue()) {
				if (count++ == 0)
					trainingQueue.push_back(std::pair<BWAPI::UnitType, int>(tunit, unit->getRemainingTrainTime()));
				else
					trainingQueue.push_back(std::pair<BWAPI::UnitType, int>(tunit, -1));
			}
		}

	}

	// draw the frame count
	BWAPI::Broodwar->drawTextScreen(10, 10, "Frame: %d", BWAPI::Broodwar->getFrameCount());

	// draw the train queue
	int x = 14;
	for (const auto & p : trainingQueue){
		x += 10;
		BWAPI::Broodwar->drawTextScreen(10, 10 + x, "%s: %d",p.first.getName().c_str(), p.second);
	}

	trainingQueue.clear();


	// draw the lanes
	// NB. The dimensions listed below are mere guesses. 
	for (unsigned i = 0; i != lanes_; i++) {
		BWAPI::Broodwar->drawTextScreen(500 - i * 20, 50, lane_names_[i].c_str());
		BWAPI::Broodwar->drawTextScreen(500 -  i*20, 60, lane_stream_[i].str().c_str());
	}

}


