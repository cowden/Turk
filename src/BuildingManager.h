#pragma once
#include "Common.h"


namespace Turk {

	// Commander Class
	class BuildingManager
	{

	public:
		BuildingManager();
		~BuildingManager(){};


		//
		void PythonBuildingLocation();


		// Determine the building location depending on the map info
		void MapConnector();

		// $$$$$$$$$    Building Positining        $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		// Determine the location of pylons
		void PylonPositioning(BWAPI::TilePosition, int);

		// Determine the location of gateways
		void GateWayPositioning(BWAPI::TilePosition, int);

		// Determine the location of technical building
		void TechBuildingPositioning(BWAPI::TilePosition, int);


		// $$$$$$$$ Special Pylon Buliding Locations     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		// Build pylons at both ends of minerals
		void DefensePylonLocation(BWAPI::Unitset);

		void ChokeDefensePylon();

		// $$$$$$$$$$$$$$$$$$$$$$$$$ Class Variable Handler $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		// Pylon Location Handler
		BWAPI::TilePosition PylonLocationPresent();
		void PylonLocationSaver(std::vector<BWAPI::TilePosition>);
		void PylonLocationRemover();

		// GateWay Location Handler
		BWAPI::TilePosition GateWayLocationPresent();
		void GateWayLocationSaver(std::vector<BWAPI::TilePosition>);
		void GateWayLocationRemover();

		// Tech Building Locations
		BWAPI::TilePosition TechLocationPresent();
		void TechLocationSaver(std::vector<BWAPI::TilePosition>);
		void TechLocationRemover();

		BWAPI::TilePosition GetExpansionBase();

		// Building Function
		void MrBuilder(BWAPI::Unit, BWAPI::UnitType, BWAPI::TilePosition);

		static BuildingManager &Instance();

	private:
		// Pylon Location Vector
		std::vector<BWAPI::TilePosition>	m_PylonTilePosition;

		// GateWay Location Vector
		std::vector<BWAPI::TilePosition>	m_GateTilePosition;

		// Technical buiding location Vector
		std::vector<BWAPI::TilePosition>	m_TechBuildingTileLocation;


		// CSC - Why is this here??
		// Buildings	
		BWAPI::UnitType Pylon = BWAPI::UnitTypes::Protoss_Pylon;
		BWAPI::UnitType GateWay = BWAPI::UnitTypes::Protoss_Gateway;
		BWAPI::UnitType Nexus = BWAPI::UnitTypes::Protoss_Nexus;
		BWAPI::UnitType Assimilator = BWAPI::UnitTypes::Protoss_Assimilator;
		BWAPI::UnitType Forge = BWAPI::UnitTypes::Protoss_Forge;
		BWAPI::UnitType Cybernetics = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
		BWAPI::UnitType CitadelOfAdun = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
		BWAPI::UnitType TemplerArchive = BWAPI::UnitTypes::Protoss_Templar_Archives;
		BWAPI::UnitType Stargate = BWAPI::UnitTypes::Protoss_Stargate;

		// Units
		BWAPI::UnitType Zealot = BWAPI::UnitTypes::Protoss_Zealot;
		BWAPI::UnitType Dragoon = BWAPI::UnitTypes::Protoss_Dragoon;
		BWAPI::UnitType Corsair = BWAPI::UnitTypes::Protoss_Corsair;
	
	};

} // end namespace Turk