#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <windows.h>
#include "Commander.h"


DWORD WINAPI AnalyzeThread();


// Remember not to use "Broodwar" in any global class constructor!

namespace Turk {

	class TheTurk : public BWAPI::AIModule
	{

		Commander _Commander;

		// Check the Build Order Miles Stone
		bool FirstGateExist = false;

		bool FirstGasExist = false;
		bool FirstCybernetics = false;
		bool FirstAdun = false;


		// Upgrade Situation: One Time and gone forever
		bool Singularity = false;
		bool Leg_Enhancements = false;



		// Buildings
		BWAPI::UnitType Pylon = BWAPI::UnitTypes::Protoss_Pylon;
		BWAPI::UnitType GateWay = BWAPI::UnitTypes::Protoss_Gateway;
		BWAPI::UnitType Nexus = BWAPI::UnitTypes::Protoss_Nexus;
		BWAPI::UnitType Assimilator = BWAPI::UnitTypes::Protoss_Assimilator;

		// Units
		BWAPI::UnitType Zealot = BWAPI::UnitTypes::Protoss_Zealot;
		BWAPI::UnitType Dragoon = BWAPI::UnitTypes::Protoss_Dragoon;
		BWAPI::UnitType Corsair = BWAPI::UnitTypes::Protoss_Corsair;


		int PylonLimiter = 0;




		int MaxGateWayCount = 3;
		int StartClockPosition = 0;



		// Building Location Variables
		int BaseToPylon = 1;
		int PylonToChoke = 2;




		// ===================================================
		// Gas Collector Information
		int WorkersPerGas = 3;
		// ===================================================




		// System Parameters
		unsigned MaxWorkerCount = 40;
		int MaxMineralDist = 400;





		// Building Positions
		BWAPI::Position homePosition;
		BWAPI::TilePosition homeTilePosition;

		BWAPI::TilePosition FirstPylonTilePosition;
		BWAPI::TilePosition SecondPylonTilePosition;
		BWAPI::TilePosition ThirdPylonTilePosition;
		std::vector<BWAPI::TilePosition>	PylonTilePosition;


		BWAPI::TilePosition FirstGateLocation;
		BWAPI::TilePosition SecondGateLocation;
		BWAPI::TilePosition ThirdGateLocation;
		std::vector<BWAPI::TilePosition>	GateTilePosition;


		BWAPI::Position HillPosition;


		// Units
		BWAPI::Unit Scout = nullptr;
		BWAPI::Unit ResourceDepot = nullptr;




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

	};

} // end Turk namespace