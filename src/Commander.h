#pragma once
#include "Common.h"


namespace Turk {

	// Commander Class
	class Commander
	{


		BWAPI::Unitset _ValidUnits;
		BWAPI::Unitset _BaseUnits;
		BWAPI::Unitset _WorkerUnits;
		BWAPI::Unitset _ScoutUnits;

		// A set of minerals: It is initiated at every starting and expansion
		BWAPI::Unitset MineralSets;




		std::map<std::string, int>	UnitCount;





	public:
		Commander();
		~Commander(){};


		// Unit Collector
		void ValidUnitCollector(BWAPI::Unit);
		bool IsValidUnit(BWAPI::Unit);

		BWAPI::Unitset MineralCollector(BWTA::BaseLocation * StartingPoint);
		BWAPI::Unitset MineralPresent();
		BWAPI::Unitset UnitSetPresent();
		BWAPI::Unitset BasePresent();
		BWAPI::Unitset WorkerPresent();


		// Save the locations of minerals around our bases
		void MineralSaver(BWAPI::Unitset);


		// A resource depot is a Command Center, Nexus, or Hatchery
		// A resource depot will generate workers up to the maximum number of workers
		void ProbeMaker(unsigned);

		// Make probes work
		void ProbeWork(int);

		// Scouter managenet
		void ScoutHander(BWAPI::Unit Scout);

		// 
		std::map<std::string, int> Commander::UnitCounter();

		static Commander &Instance();


	};






























	//
	//class UnitAssign
	//{
	//public:
	//	BWAPI::Unit unit;
	//	bool isAssigned;
	//
	//	UnitAssign(BWAPI::Unit u){
	//		unit = u;
	//		isAssigned = false;
	//	}		


} // end Turk namespace