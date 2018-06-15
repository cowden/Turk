#pragma once


#include "HUD.h"
#include "Logger.h"
#include "ConfigDB.h"
#include "Common.h"

#include "sqlite3.h"



DWORD WINAPI AnalyzeThread();


// Remember not to use "Broodwar" in any global class constructor!

namespace Turk {
class TheTurk : public BWAPI::AIModule
{

public:

	TheTurk();

	// Virtual functions for callbacks, leave these as they are.
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onSendText(std::string text);
	virtual void onReceiveText(BWAPI::Player player, std::string text);
	virtual void onPlayerLeft(BWAPI::Player player);
	virtual void onNukeDetect(BWAPI::Position target);
	virtual void onUnitDiscover(BWAPI::Unit unit);
	virtual void onUnitEvade(BWAPI::Unit unit);
	virtual void onUnitShow(BWAPI::Unit unit);
	virtual void onUnitHide(BWAPI::Unit unit);
	virtual void onUnitCreate(BWAPI::Unit unit);
	virtual void onUnitDestroy(BWAPI::Unit unit);
	virtual void onUnitMorph(BWAPI::Unit unit);
	virtual void onUnitRenegade(BWAPI::Unit unit);
	virtual void onSaveGame(std::string gameName);
	virtual void onUnitComplete(BWAPI::Unit unit);
	void drawTerrainData();
	// Everything below this line is safe to modify.
	
private:

	// Heads Up Display & Visualization
	HUD m_hud;

	// Logger
	Logger * m_log;

	// Sqlite3 configuration database connection
	ConfigDB * m_db;

	// name of the class
	static const char * m_name;

};  // end TheTurk class declaration


} // end Turk namespace
