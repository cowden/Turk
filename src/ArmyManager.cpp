
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
	scout_->setVerbosity(verbose_);
	umanity.request(BWAPI::Broodwar->self()->getRace().getWorker(), scout_);
}

void ArmyManager::loadModel(const model_args & args) {

	// load the model load
	std::string msg;
	msg = "Loading Army Model";
	Turk::Logger::instance()->log(m_name.c_str(), msg.c_str());

	// load the model
	std::stringstream model_loc;
	model_loc << std::getenv("TURKDIR") << "\\data\\Terran\\ArmyManager" << "\\Terran_basic_army.txt";
	initialize_model(model_loc.str());
}

void ArmyManager::initialize_model(const std::string & name) {

	// load the model load
	std::stringstream msg;
	msg << "Loading army model " << name;
	Turk::Logger::instance()->log(m_name.c_str(), msg.str().c_str());

	msg.clear();
	msg.str(std::string());

	// open the file
	std::ifstream in(name.c_str(), std::ifstream::in);
	int err = errno;

	if (!in.is_open()) {
		msg << "There was an error loading the army model! " << err << " " << std::strerror(err);
		Turk::Logger::instance()->log(m_name.c_str(), msg.str().c_str());

		in.close();
		return;
	}

	// parse the file
	char txt[500];
	in.getline(txt, 500);
	while (!in.eof() && in.good() && in.is_open()) {

		// parse the line
		std::stringstream ss(txt);

	}

	// close the file
	in.close();
}