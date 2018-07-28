
#include "HUD.h"


using namespace Turk;
using namespace BWAPI;


HUD & HUD::Instance(){
	static HUD instance;
	return instance;
}

void HUD::drawInterface(){


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
			trainingQueue.push_back(std::pair<BWAPI::UnitType, int>(ut, unit->getRemainingTrainTime()));
		}

	}

	// draw the frame count
	BWAPI::Broodwar->drawTextScreen(10, 10, "Frame: %d", BWAPI::Broodwar->getFrameCount());

	// draw the train queue
	int x = 14;
	for (const auto & p : trainingQueue){
		x += 4;
		BWAPI::Broodwar->drawTextScreen(10, 10 + x, "%d: %d",p.first.getID(), p.second);
	}

	trainingQueue.clear();


}


/*const std::vector<BWAPI::Unit> & HUD::getUnits(){

}*/
