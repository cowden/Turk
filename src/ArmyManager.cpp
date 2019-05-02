
#include "ArmyManager.h"

#include "Logger.h"

#include "Scout.h"

using namespace Turk;

unsigned ArmyManager::m_nArmies = 0U;

int ArmyManager::execute(int command, const bot_args & args) {

	// initiate scout
	if (command | INITIATE_SCOUT) {
		initiate_scout();
	}

	return 0;
}

void ArmyManager::initiate_scout() {
	scout_ = new Scout();
	umanity.request(BWAPI::Broodwar->self()->getRace().getWorker(), scout_);
}