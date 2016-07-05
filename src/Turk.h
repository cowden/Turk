#pragma once

#include "BuildingManager.h"
#include "ScoutManager.h"
#include "Common.h"



DWORD WINAPI AnalyzeThread();


// Remember not to use "Broodwar" in any global class constructor!

namespace Turk {
class TheTurk : public BWAPI::AIModule
{

public:

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

	// map analysis
	bool m_analyzed;
	bool m_analysis_just_finished;

	// Taken from Commander
	BWAPI::Unitset m_ValidUnits;
	BWAPI::Unitset m_BaseUnits;
	BWAPI::Unitset m_WorkerUnits;
	BWAPI::Unitset m_ScoutUnits;

	// A set of minerals: It is initiated at every starting and expansion
	BWAPI::Unitset m_MineralSets;
	
	std::map<std::string, int>	m_UnitCount;

	// Unit Collector
	void ValidUnitCollector(const BWAPI::Unit &);
	bool IsValidUnit(const BWAPI::Unit &);

	const BWAPI::Unitset & MineralCollector(const BWTA::BaseLocation * StartingPoint);
	const BWAPI::Unitset & MineralPresent();
	const BWAPI::Unitset & UnitSetPresent();
	const BWAPI::Unitset & BasePresent();
	const BWAPI::Unitset & WorkerPresent();


	// Save the locations of minerals around our bases
	void MineralSaver(const BWAPI::Unitset &);

	// A resource depot is a Command Center, Nexus, or Hatchery
	// A resource depot will generate workers up to the maximum number of workers
	void ProbeMaker(unsigned);

	// Make probes work
	void ProbeWork(int);

	// Scouter managenet
	void ScoutHander(const BWAPI::Unit & Scout);

	// 
	const std::map<std::string, int> & UnitCounter();
	// end of commander section

	// Check the Build Order Miles Stone
	bool m_FirstExpansion = false;
	bool m_FirstGasExist = false;
	bool m_FirstCybernetics = false;
	bool m_FirstAdun = false;
	bool m_FirstFleetBeacon = false;


	// Upgrade Situation: One Time and gone forever
	bool m_Singularity = false;
	bool m_Leg_Enhancements = false;
	
	// Scouting Information
	bool m_EnemyDetection = false;
	bool m_CorsairSearchMode = false;


	// CSC - Why???
	BWAPI::UnitType Pylon = BWAPI::UnitTypes::Protoss_Pylon;
	BWAPI::UnitType GateWay = BWAPI::UnitTypes::Protoss_Gateway;
	BWAPI::UnitType Nexus = BWAPI::UnitTypes::Protoss_Nexus;
	
	
	// Buildings	
	BWAPI::UnitType Assimilator = BWAPI::UnitTypes::Protoss_Assimilator;
	BWAPI::UnitType Forge = BWAPI::UnitTypes::Protoss_Forge;
	BWAPI::UnitType Cybernetics = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	BWAPI::UnitType CitadelOfAdun = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
	BWAPI::UnitType TemplerArchive = BWAPI::UnitTypes::Protoss_Templar_Archives;
	BWAPI::UnitType StarGate = BWAPI::UnitTypes::Protoss_Stargate;
	BWAPI::UnitType FleetBeacon = BWAPI::UnitTypes::Protoss_Fleet_Beacon;

	// Units
	BWAPI::UnitType Zealot = BWAPI::UnitTypes::Protoss_Zealot;
	BWAPI::UnitType Dragoon = BWAPI::UnitTypes::Protoss_Dragoon;
	BWAPI::UnitType Corsair = BWAPI::UnitTypes::Protoss_Corsair;

	int m_PylonLimiter = 0;
		
	int m_MaxGateWayCount = 1;	
	int m_StartClockPosition = 0;

	// Building Location Variables
	int m_BaseToPylon = 5;
	int m_PylonToChoke = 1;

	// ===================================================
	// Gas Collector Information
	int m_WorkersPerGas = 3;	
	// ===================================================

	// System Parameters
	unsigned m_MaxWorkerCount = 30;
	int m_MaxMineralDist = 400;

	// Building Positions
	BWAPI::Position m_homePosition;
	BWAPI::TilePosition m_homeTilePosition;
	BWAPI::Position m_FirstExpansionPosition;
	BWAPI::Position m_Campus;
	BWAPI::Position m_HillPosition;
	BWAPI::TilePosition m_NextExpansion;
	
	// Enemy Building Position
	BWAPI::Position m_EnemyHomeBase;
	BWAPI::Position m_EnemyHillPosition;
	BWAPI::Position m_EnemyExpansion;


	// $$$$$$$$$$$$$$$$$$$$
	// War Flags
	bool m_CorsairAttackTriger = false;
	bool m_EnemyBaseUpdate = false;


	// Units
	// CSC - Why??
	BWAPI::Unit ResourceDepot = nullptr;

};  // end TheTurk class declaration

} // end Turk namespace
