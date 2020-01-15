#include "Turk.h"

#include <boost/archive/text_iarchive.hpp>

#include <iostream>
#include <fstream>

using namespace BWAPI;
using namespace Filter;
using namespace Turk;

// Define static member data
const char * TheTurk::m_name = "TheTurk";
Logger * Logger::m_ptr = 0;

// define the empty artie
ArtieInterface Turk::artie;

//
// default constructor
TheTurk::TheTurk() {

	char * tdir = std::getenv("TURKDIR");
	std::string baseDir(tdir);
	std::string logDir("\\logs\\TurkTest");

	// connect to logger
	m_log = Turk::Logger::instance();
	m_log->newLog(std::string(baseDir+logDir).c_str());

	//DB connection
	m_log->log(m_name, "Connecting to DB");
	m_db = Turk::ConfigDB::instance();

	char msg[500];
	sprintf(msg, "Opened sqlite3 connection");
	m_log->log(m_name, msg);

	m_log->log(m_name, "Get LogDir");
	std::vector<std::string> res = m_db->query("SELECT value FROM config WHERE key = 'LogDir';");
	for (auto r : res) {
		m_log->log(m_name, r.c_str());
	}
	

	m_log->log(m_name, "Get BaseName");
	res = m_db->query("SELECT value FROM config WHERE key = 'LogBase';");
	for (auto r : res) {
		m_log->log(m_name, r.c_str());
	}
	

	// get the verbosity level
	m_log->log(m_name, "Get Verbosity Level");
	res = m_db->query("SELECT value FROM config WHERE key = 'Verbosity';");
	verbose_ = atoi(res[0].c_str());

	// load the tech tree
	weaver.load();
	

}

void TheTurk::onStart()
{

	// Hello World!
	//Broodwar->sendText("Hello world!");
	Broodwar->setLocalSpeed(1);

	char msg[500];
	sprintf(msg, ">>>> Starting New Game <<<<");
	m_log->log(m_name, msg);

	// Print the map name.
	// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
	Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;
	sprintf(msg, "map: %s", Broodwar->mapFileName());
	m_log->log(m_name, msg);
	
	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);

	// Uncomment the following line and the bot will know about everything through the fog of war (cheat).
	//Broodwar->enableFlag(Flag::CompleteMapInformation);

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);

    // Determine the map and load the appropriate ARTIE
	// get the map name
	std::string mapName = Broodwar->mapFileName();
	std::string artName;
	artName = std::string(std::getenv("TURKDIR")) + "\\data\\artie_data\\" + mapName.substr(0,mapName.length()-4) + "_artie.txt";

	sprintf(msg, "ARTIE: %s", artName);
	m_log->log(m_name, msg);

	// load the analyzed map
	artie.load_artie(artName);

	// log some information about the ARITE object
	if (verbose_ > 0) {
		m_log->log(m_name, "Logging ARTIE graph");
		m_log->log(m_name, "index  x  y  choke  artic");
		const std::vector<unsigned> & chokes = artie.getARTIE().get_chokes();
		const unsigned N = chokes.size();
		for (unsigned i = 0; i != N; i++) {
			const Turk::region & reg = artie.getARTIE()[i];

			std::stringstream mss;
			mss << i << " " << reg.position().x << " " << reg.position().y << " " << reg.is_choke() << " " << reg.is_articulation();
			m_log->log(m_name, mss.str().c_str());
		}
	}


	// Check if this is a replay
	if (Broodwar->isReplay()){
		// Announce the players in the replay
		Broodwar << "The following players are in this replay:" << std::endl;
		m_log->log(m_name, "Players in this replay:");

		// Iterate all the players in the game using a std:: iterator
		Playerset players = Broodwar->getPlayers();
		for (auto p : players)
		{
			// Only print the player if they are not an observer
			if (!p->isObserver()) {
				Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
				sprintf(msg, "%s, playing as %s", p->getName(), p->getRace().c_str());
				m_log->log(m_name, msg);
			}
		}

	}
	else // if this is not a replay
	{
		// Retrieve you and your enemy's races. enemy() will just return the first enemy.
		// If you wish to deal with multiple enemies then you must use enemies().
		if (Broodwar->enemy()) { // First make sure there is an enemy
			Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
			sprintf(msg, "Matchup: %s vs %s", Broodwar->self()->getRace().c_str(), Broodwar->enemy()->getRace().c_str());
			m_log->log(m_name, msg);
		}

		// start the strategy managery and itialize the models
		strat_man_.setVerbosity(verbose_);
		strat_man_.initialize(Broodwar->self()->getRace(), Broodwar->enemy()->getRace());
	}



}



void TheTurk::onFrame(){
	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;
	// draw the HUD
	//m_hud.drawInterface();
	HUD::Instance().drawInterface();

	
	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;
	
	// process UnitManager
	umanity.process();

	

	// process the bot
	strat_man_.process();
}




void TheTurk::onSendText(std::string text)
{

	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());


	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!

}

void TheTurk::onReceiveText(BWAPI::Player player, std::string text)
{
	// Parse the received text
	Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void TheTurk::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void TheTurk::onNukeDetect(BWAPI::Position target)
{

	// Check if the target is a valid position
	if (target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar->sendText("Where's the nuke?");
	}

	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void TheTurk::onUnitDiscover(BWAPI::Unit unit)
{
}

void TheTurk::onUnitEvade(BWAPI::Unit unit)
{
}

void TheTurk::onUnitShow(BWAPI::Unit unit)
{
}

void TheTurk::onUnitHide(BWAPI::Unit unit)
{
}

void TheTurk::onUnitCreate(BWAPI::Unit unit)
{
	// pass to unit manager
	umanity.onUnitCreate(unit);
}




void TheTurk::onUnitDestroy(BWAPI::Unit unit)
{
	// call unit manager to update loss to agents
	umanity.onUnitDestroy(unit);

}

void TheTurk::onUnitMorph(BWAPI::Unit unit)
{
	// catch construction on a geyser
	umanity.onUnitCreate(unit);
}

void TheTurk::onUnitRenegade(BWAPI::Unit unit)
{
}

void TheTurk::onSaveGame(std::string gameName)
{
	Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void TheTurk::onUnitComplete(BWAPI::Unit unit)
{
	// pass this to Umanity
	umanity.onUnitComplete(unit);
}




void TheTurk::onEnd(bool isWinner)
{
	// Called when the game ends
	if (isWinner)
	{
		// Log your win here!
	}
}




