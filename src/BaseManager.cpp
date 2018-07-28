 
#include "BaseManager.h"

#include "Logger.h"


using namespace Turk;

unsigned BaseManager::m_nBases = 0U;

void BaseManager::loadModel(const model_args & args) {

	// log the model load 
	std::string msg;
	msg = "Loading model";
	Turk::Logger::instance()->log(m_name.c_str(),msg.c_str());

	// set this bot's race (load specific techtree)
	race_ = args.trace_;

	// map race's units

	// load the initial build order

}