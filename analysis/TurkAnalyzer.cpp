/////////////////////////////////////////////
// C S Cowden
// The Turk - 
/////////////////////////////////////////////


#include "TurkAnalyzer.h"
#include <BWAPI.h>
#include <iostream>
#include <limits>
#include <Windows.h>

bool analyzed;
bool analysis_just_finished;

using namespace BWAPI;
using namespace Filter;
using namespace Turk;

bool FirstGateExist = false;
bool FirstPylonExist = false;
bool FirstGasExist = false;
bool FirstCybernetics = false;
bool Singularity = false;
bool FirstAdun = false;
bool Leg_Enhancements = false;
int GasWorkerCount = 0;
int WorkersPerGas = 3;
int GateWayCount = 0;
int ProbeCount = 0;
int ZealotCount = 0;
int DragoonCount = 0;
int PylonLimiter = 0;
int MaxGateWayCount = 3;



// Building Type
UnitType GateWay = BWAPI::UnitTypes::Protoss_Gateway;
UnitType Assimilator = BWAPI::UnitTypes::Protoss_Assimilator;
UnitType Cybernetics = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
UnitType CitadelOfAdun = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
UnitType Nexus = BWAPI::UnitTypes::Protoss_Nexus;


BWAPI::Position Enterance;
BWAPI::Position SecondField;
BWAPI::TilePosition NexusLocation;


void TurkAnalyzer::onStart()
{
	// Hello World!
	Broodwar->sendText("Duehee Lee Modified!");

	Broodwar->setLocalSpeed(15);

	// Print the map name.
	// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
	Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);

	// Uncomment the following line and the bot will know about everything through the fog of war (cheat).
	//Broodwar->enableFlag(Flag::CompleteMapInformation);

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);

	// Check if this is a replay
	if (Broodwar->isReplay())
	{

		// Announce the players in the replay
		Broodwar << "The following players are in this replay:" << std::endl;

		// Iterate all the players in the game using a std:: iterator
		Playerset players = Broodwar->getPlayers();
		for (auto p : players)
		{
			// Only print the player if they are not an observer
			if (!p->isObserver())
				Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
		}

	}
	else // if this is not a replay
	{
		// Retrieve you and your enemy's races. enemy() will just return the first enemy.
		// If you wish to deal with multiple enemies then you must use enemies().
		if (Broodwar->enemy()) // First make sure there is an enemy
			Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
	}





	BWAPI::Position homePosition = BWAPI::Position(Broodwar->self()->getStartLocation());

	// 12
	if (homePosition.y < 500)
	{
		Enterance.x = 2028;
		Enterance.y = 116;
		SecondField.x = 1568;
		SecondField.y = 448;
	}
	// 3
	else if (homePosition.x > 3000)
	{
		Enterance.x = 3812;
		Enterance.y = 1820;
		SecondField.x = 3488;
		SecondField.y = 2112;
	}
	// 6
	else if (homePosition.y > 3000)
	{
		Enterance.x = 2056;
		Enterance.y = 3928;
		SecondField.x = 2336;
		SecondField.y = 3616;
	}
	// 9
	else if (homePosition.x < 500)
	{
		Enterance.x = 200;
		Enterance.y = 2240;
		SecondField.x = 480;
		SecondField.y = 1984;
	}


	analyzed = true;
	analysis_just_finished = true;

}

void TurkAnalyzer::onEnd(bool isWinner)
{
	// Called when the game ends
	if (isWinner)
	{
		// Log your win here!
	}
}








void TurkAnalyzer::onFrame()
{
	// Called once every game frame

	// Display the game frame rate as text in the upper left area of the screen
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	// Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS() );

	//Broodwar->drawTextScreen(200, 20, "Number of Units: %d", BWAPI::Broodwar->self()->supplyUsed() / 2 );
	//Broodwar->drawTextScreen(200, 40, "Number of Units: %d", BWAPI::Broodwar->self()->supplyTotal() / 2 );


	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	//BWTA draw
	if (analyzed){
		drawTerrainData();
	}


	if (analysis_just_finished)
	{
		Broodwar << "Finished analyzing map." << std::endl;;
		analysis_just_finished = false;
	}


	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;


	// Iterate through all the units that we own


	// Count the number of probes
	ProbeCount = 0;
	ZealotCount = 0;
	DragoonCount = 0;
	GateWayCount = 0;
	for (auto & unit2 : Broodwar->self()->getUnits())
	{
		if (!unit2->exists())
			continue;

		// Ignore the unit if it is incomplete or busy constructing
		if (!unit2->isCompleted() || unit2->isConstructing())
			continue;

		// If the unit is a worker unit
		if (unit2->getType().isWorker())
		{
			ProbeCount = ProbeCount + 1;
		}
		else if (unit2->getType() == BWAPI::UnitTypes::Protoss_Zealot)
		{
			ZealotCount = ZealotCount + 1;
		}
		else if (unit2->getType() == BWAPI::UnitTypes::Protoss_Dragoon)
		{
			DragoonCount = DragoonCount + 1;
		}
		else if (unit2->getType() == GateWay)
		{
			GateWayCount = GateWayCount + 1;
		}
	}

	Broodwar->drawTextScreen(100, 10, "Number of Zealot: %d", ZealotCount);
	Broodwar->drawTextScreen(100, 30, "Number of Dragoon: %d", DragoonCount);







	// Iterate through all the units that we own
	for (auto &u : Broodwar->self()->getUnits())
	{
		// Ignore the unit if it no longer exists
		// Make sure to include this block when handling any Unit pointer!
		if (!u->exists())
			continue;

		// Ignore the unit if it has one of the following status ailments
		if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
			continue;

		// Ignore the unit if it is in one of the following states
		if (u->isLoaded() || !u->isPowered() || u->isStuck())
			continue;

		// Ignore the unit if it is incomplete or busy constructing
		if (!u->isCompleted() || u->isConstructing())
			continue;


		// Finally make the unit do some stuff!




		// Build a gateway or generate a zealot	
		if (u->getType().isBuilding())
		{
			// We found a gateway, and it is not generating a unit
			if (u->isIdle() && u->getType() == GateWay)
			{
				u->rightClick(Enterance);

				//BWAPI::Position Hill = BWTA::getNearestChokepoint(BWAPI::Position(Broodwar->self()->getStartLocation()))->getCenter();
				//u->rightClick(Hill);


				if (FirstCybernetics && ZealotCount >= DragoonCount){
					u->train(BWAPI::UnitTypes::Protoss_Dragoon);
				}
				else{
					u->train(BWAPI::UnitTypes::Protoss_Zealot);
				}

			}

			// We found a gateway, and it is generating a unit.
			else if (!u->isIdle() && u->getType() == GateWay && GateWayCount < MaxGateWayCount && FirstCybernetics)
			{
				static int lastChecked = 0;

				// If we are supply blocked and haven't tried constructing more recently
				if (lastChecked + 400 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(GateWay) == 0)
				{
					// Frame Count
					lastChecked = Broodwar->getFrameCount();

					// Retrieve a unit that is capable of constructing the supply needed
					Unit GateBuilder = u->getClosestUnit(GetType == GateWay.whatBuilds().first && (IsIdle || IsGatheringMinerals) && IsOwned);

					// If a unit was found
					if (GateBuilder)
					{
						TilePosition targetBuildLocation = Broodwar->getBuildLocation(GateWay, GateBuilder->getTilePosition());
						if (targetBuildLocation)
						{
							// Order the builder to construct the supply structure				
							GateBuilder->build(GateWay, targetBuildLocation);

						}
					}
				}
			}
		}







		/*
		if (Broodwar->self())
		{
		Unitset myUnits = Broodwar->self()->getUnits();
		for (auto u : myUnits)
		{
		if (u->isIdle() && u->getType().isResourceDepot())
		u->train(u->getType().getRace().getWorker());
		}
		}
		*/




		// If the unit is a worker unit
		if (u->getType().isWorker())
		{
			// if our worker is idle
			if (u->isIdle())
			{
				// Order workers carrying a resource to return them to the center,
				// otherwise find a mineral patch to harvest.
				if (u->isCarryingGas() || u->isCarryingMinerals())
				{
					u->returnCargo();
				}
				else if (!u->getPowerUp())  // The worker cannot harvest anything if it
				{                             // is carrying a powerup such as a flag
					// Harvest from the nearest mineral patch or gas refinery
					if (!u->gather(u->getClosestUnit(IsMineralField))) // IsRefinery
					{
						// If the call fails, then print the last error message
						Broodwar << Broodwar->getLastError() << std::endl;
					}
				} // closure: has no powerup

			} // closure: if idle

		}
		else if (u->getType().isResourceDepot()) // A resource depot is a Command Center, Nexus, or Hatchery
		{

			// Order the depot to construct more workers! But only when it is idle.
			if (u->isIdle() && ProbeCount < 40) // && !u->train(u->getType().getRace().getWorker())
			{
				u->train(u->getType().getRace().getWorker());
				// If that fails, draw the error at the location so that you can visibly see what went wrong!
				// However, drawing the error once will only appear for a single frame
				// so create an event that keeps it on the screen for some frames

				// Position homePosition = Position(Broodwar->self()->getStartLocation());
			}
			Position pos = u->getPosition();
			Error lastErr = Broodwar->getLastError();
			Broodwar->registerEvent([pos, lastErr](Game*){ Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
				nullptr,    // condition
				Broodwar->getLatencyFrames());  // frames to run

			// Retrieve the supply provider type in the case that we have run out of supplies		
			UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
			static int lastChecked = 0;


			// If we are supply blocked and haven't tried constructing more recently
			if (lastErr == Errors::Insufficient_Supply &&
				lastChecked + 300 < Broodwar->getFrameCount() &&
				Broodwar->self()->incompleteUnitCount(supplyProviderType) <= PylonLimiter)
			{
				lastChecked = Broodwar->getFrameCount();

				// Retrieve a unit that is capable of constructing the supply needed
				Unit supplyBuilder = u->getClosestUnit(GetType == supplyProviderType.whatBuilds().first &&
					(IsIdle || IsGatheringMinerals) &&
					IsOwned);

				// If a unit was found
				if (supplyBuilder)
				{
					if (supplyProviderType.isBuilding())
					{
						TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
						if (targetBuildLocation)
						{
							// Register an event that draws the target build location
							Broodwar->registerEvent([targetBuildLocation, supplyProviderType](Game*)
							{
								Broodwar->drawBoxMap(Position(targetBuildLocation),
									Position(targetBuildLocation + supplyProviderType.tileSize()),
									Colors::Blue);
							},
								nullptr,  // condition
								supplyProviderType.buildTime() + 100);  // frames to run

							// Order the builder to construct the supply structure				
							supplyBuilder->build(supplyProviderType, targetBuildLocation);

						}
					}
					else
					{
						// Train the supply provider (Overlord) if the provider is not a structure
						supplyBuilder->train(supplyProviderType);
					}
				} // closure: supplyBuilder is valid
			} // closure: insufficient supply     

			// If there is the first pylon,but it there is no gateway, please build a gateway.s
			else if (FirstPylonExist && !FirstGateExist)
			{
				static int lastChecked = 0;

				// If we are supply blocked and haven't tried constructing more recently
				if (lastChecked + 400 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(GateWay) == 0)
				{
					// Frame Count
					lastChecked = Broodwar->getFrameCount();

					// Retrieve a unit that is capable of constructing the supply needed
					Unit GateBuilder = u->getClosestUnit(GetType == GateWay.whatBuilds().first && (IsIdle || IsGatheringMinerals) && IsOwned);

					if (GateBuilder)
					{
						TilePosition targetBuildLocation = Broodwar->getBuildLocation(GateWay, GateBuilder->getTilePosition());

						if (targetBuildLocation)
						{
							// Order the builder to construct the supply structure				
							GateBuilder->build(GateWay, targetBuildLocation);
						}
					}
				}
			}
			else if (FirstGateExist && !FirstGasExist)
			{

				BWAPI::TilePosition closestGeyser = BWAPI::TilePositions::None;
				double minGeyserDistanceFromHome = (std::numeric_limits<double>::max)();
				BWAPI::Position homePosition = BWAPI::Position(Broodwar->self()->getStartLocation());

				// for each geyser
				for (auto & geyser : Broodwar->getStaticGeysers())
				{
					if (geyser->getType() != BWAPI::UnitTypes::Resource_Vespene_Geyser)
					{
						continue;
					}

					BWAPI::Position geyserPos = geyser->getInitialPosition();
					BWAPI::TilePosition geyserTilePos = geyser->getInitialTilePosition();

					// check to see if it's next to one of our depots
					bool nearDepot = false;
					for (auto & unit : BWAPI::Broodwar->self()->getUnits())
					{
						if (unit->getType().isResourceDepot() && unit->getDistance(geyserPos) < 300)
						{
							nearDepot = true;
						}
					}

					if (nearDepot)
					{
						double homeDistance = geyser->getDistance(homePosition);

						if (homeDistance < minGeyserDistanceFromHome)
						{
							minGeyserDistanceFromHome = homeDistance;
							closestGeyser = geyser->getInitialTilePosition();
						}
					}


					static int lastChecked = 0;

					// If we are supply blocked and haven't tried constructing more recently
					if (lastChecked + 400 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(Assimilator) == 0)
					{
						// Frame Count
						lastChecked = Broodwar->getFrameCount();

						// Retrieve a unit that is capable of constructing the supply needed
						Unit GasBuilder = u->getClosestUnit(GetType == GateWay.whatBuilds().first && (IsIdle || IsGatheringMinerals) && IsOwned);

						if (GasBuilder)
						{
							// Order the builder to construct the supply structure				
							GasBuilder->build(Assimilator, closestGeyser);
						}
					}
				}
			}
		}

	} // closure: unit iterator


	// Build the supply unit
	if (BWAPI::Broodwar->self()->supplyUsed() / 2 + 6 > BWAPI::Broodwar->self()->supplyTotal() / 2){
		for (auto & unit2 : Broodwar->self()->getUnits()){

			if (!unit2->exists())
				continue;

			// Ignore the unit if it is incomplete or busy constructing
			if (!unit2->isCompleted() || unit2->isConstructing())
				continue;

			// If the unit is a worker unit
			if (unit2->getType().isResourceDepot()){
				UnitType supplyProviderType = unit2->getType().getRace().getSupplyProvider();
				static int lastChecked = 0;

				// If we are supply blocked and haven't tried constructing more recently
				if (lastChecked + 300 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0){
					lastChecked = Broodwar->getFrameCount();

					// Retrieve a unit that is capable of constructing the supply needed
					Unit supplyBuilder = unit2->getClosestUnit(GetType == supplyProviderType.whatBuilds().first && (IsIdle || IsGatheringMinerals) && IsOwned);

					// If a unit was found
					if (supplyBuilder){
						TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
						if (targetBuildLocation){
							// Order the builder to construct the supply structure				
							supplyBuilder->build(supplyProviderType, targetBuildLocation);
						}
					}
				}
			}
		}
	}








	// Build the Cybernetics Core
	if (FirstGasExist && !FirstCybernetics)
	{
		for (auto & unit2 : Broodwar->self()->getUnits())
		{
			// If the unit is a worker unit
			if (unit2->getType().isWorker())
			{

				static int lastChecked = 0;

				// If we are supply blocked and haven't tried constructing more recently
				if (lastChecked + 400 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(Cybernetics) == 0)
				{
					// Frame Count
					lastChecked = Broodwar->getFrameCount();

					// Retrieve a unit that is capable of constructing the supply needed
					Unit GateBuilder = unit2->getClosestUnit(GetType == Cybernetics.whatBuilds().first && (IsIdle || IsGatheringMinerals) && IsOwned);

					if (GateBuilder)
					{
						TilePosition targetBuildLocation = Broodwar->getBuildLocation(Cybernetics, GateBuilder->getTilePosition());

						if (targetBuildLocation)
						{
							// Order the builder to construct the supply structure				
							GateBuilder->build(Cybernetics, targetBuildLocation);

						}
					}
				}
			}
		}
	}


	if (FirstCybernetics && !Singularity){
		for (auto & unit : BWAPI::Broodwar->self()->getUnits())
		{
			if (unit->getType() == Cybernetics && !unit->isUpgrading())
			{
				unit->upgrade(BWAPI::UpgradeTypes::Singularity_Charge);
			}

		}
	}






	// Send workers to the gas refinary
	if (FirstGasExist && GasWorkerCount == 0)
	{
		// for each unit we have
		for (auto & unit : BWAPI::Broodwar->self()->getUnits())
		{
			// if that unit is a refinery
			if (unit->getType().isRefinery() && unit->isCompleted())
			{
				for (auto & unit2 : Broodwar->self()->getUnits())
				{
					if (GasWorkerCount == 3)
					{
						break;
					}

					// If the unit is a worker unit
					if (unit2->getType().isWorker())
					{
						GasWorkerCount = GasWorkerCount + 1;
						unit2->rightClick(unit);
					}
				}
			}
		}
	}



	// Build the Adun 
	if (FirstCybernetics && !FirstAdun && GateWayCount >= 4)
	{
		for (auto & unit2 : Broodwar->self()->getUnits())
		{
			// If the unit is a worker unit
			if (unit2->getType().isWorker())
			{
				static int lastChecked = 0;

				// If we are supply blocked and haven't tried constructing more recently
				if (lastChecked + 400 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(CitadelOfAdun) == 0)
				{
					// Frame Count
					lastChecked = Broodwar->getFrameCount();

					// Retrieve a unit that is capable of constructing the supply needed
					Unit GateBuilder = unit2->getClosestUnit(GetType == CitadelOfAdun.whatBuilds().first && (IsIdle || IsGatheringMinerals) && IsOwned);

					if (GateBuilder)
					{
						TilePosition targetBuildLocation = Broodwar->getBuildLocation(CitadelOfAdun, GateBuilder->getTilePosition());

						if (targetBuildLocation)
						{
							// Order the builder to construct the supply structure				
							GateBuilder->build(CitadelOfAdun, targetBuildLocation);
						}
					}
				}
			}
		}
	}



	// Leg Enhancements
	if (FirstAdun && !Leg_Enhancements){
		for (auto & unit : BWAPI::Broodwar->self()->getUnits())
		{
			if (unit->getType() == CitadelOfAdun && !unit->isUpgrading())
			{
				unit->upgrade(BWAPI::UpgradeTypes::Leg_Enhancements);
			}
		}
	}











	BWAPI::Position homePosition = BWAPI::Position(Broodwar->self()->getStartLocation());

	// Build the expansion nexus
	if (GateWayCount >= MaxGateWayCount)
	{
		for (auto & unit2 : Broodwar->self()->getUnits())
		{
			// If the unit is a worker unit
			if (unit2->getType().isWorker())
			{
				static int lastChecked = 0;

				// If we are supply blocked and haven't tried constructing more recently
				if (lastChecked + 400 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(Nexus) == 0)
				{
					// Frame Count
					lastChecked = Broodwar->getFrameCount();

					// Retrieve a unit that is capable of constructing the supply needed
					Unit GateBuilder = unit2->getClosestUnit(GetType == Nexus.whatBuilds().first && (IsIdle || IsGatheringMinerals) && IsOwned);

					if (GateBuilder)
					{
						
						double minDistance = 100000;

						BWAPI::TilePosition homeTile = Broodwar->self()->getStartLocation();

						

						if (NexusLocation){
							// Order the builder to construct the supply structure				
							GateBuilder->build(Nexus, NexusLocation);
						}
					}
				}
			}
		}
	}

	// If the number of Nexus is greater or equal to 2, increase the number of gates.
	int NofNexus = 0;
	for (auto & unit2 : Broodwar->self()->getUnits())
	{
		if (unit2->getType().isBuilding() && unit2->getType() == Nexus)
		{
			NofNexus = NofNexus + 1;
		}
		if (NofNexus >= 2){
			MaxGateWayCount = 8;
			break;
		}
	}






}





DWORD WINAPI AnalyzeThread()
{
	analyzed = true;
	analysis_just_finished = true;
	return 0;
}




void TurkAnalyzer::drawTerrainData()
{
	
}


void TurkAnalyzer::onSendText(std::string text)
{
	if (text == "/analyze") {
		if (analyzed == false) {
			Broodwar << "Analyzing map... this may take a minute" << std::endl;;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
		}
	}
	else {
		// Send the text to the game if it is not being processed.
		Broodwar->sendText("%s", text.c_str());
	}
}

void TurkAnalyzer::onReceiveText(BWAPI::Player player, std::string text)
{
	// Parse the received text
	Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void TurkAnalyzer::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void TurkAnalyzer::onNukeDetect(BWAPI::Position target)
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

void TurkAnalyzer::onUnitDiscover(BWAPI::Unit unit)
{
}

void TurkAnalyzer::onUnitEvade(BWAPI::Unit unit)
{
}

void TurkAnalyzer::onUnitShow(BWAPI::Unit unit)
{
}

void TurkAnalyzer::onUnitHide(BWAPI::Unit unit)
{
}

void TurkAnalyzer::onUnitCreate(BWAPI::Unit unit)
{
	//if ( Broodwar->isReplay() )
	//{   /Protoss building starts
	// if we are in a replay, then we will print out the build order of the structures
	//if ( unit->getType().isWorker()  )
	//{
	//Position pos = unit->getPosition();
	//int seconds = Broodwar->getFrameCount()/24;
	//int minutes = seconds/60;
	//seconds %= 60;
	//Position pos = Position(NexusLocation);
	//Broodwar->sendText("Loc: %d,-%d ", pos);
	//}
	//}
}

void TurkAnalyzer::onUnitDestroy(BWAPI::Unit unit)
{
}

void TurkAnalyzer::onUnitMorph(BWAPI::Unit unit)
{
	//  if ( Broodwar->isReplay() )
	// {  // Zerg Building Morphing
	// if we are in a replay, then we will print out the build order of the structures
	if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
	{
		int seconds = Broodwar->getFrameCount() / 24;
		int minutes = seconds / 60;
		seconds %= 60;
		Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
	}
	// }
}

void TurkAnalyzer::onUnitComplete(BWAPI::Unit unit)
{
	if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
	{
		Position pos = unit->getPosition();
		//int seconds = Broodwar->getFrameCount() / 24;
		//int minutes = seconds / 60;
		//seconds %= 60;
		Broodwar->sendText("%.2d:%.2d: %s completed a %s", pos, unit->getPlayer()->getName().c_str(), unit->getType().c_str());

		if (unit->getType() == BWAPI::UnitTypes::Protoss_Gateway)
		{
			FirstGateExist = true;
		}
		else if (unit->getType() == BWAPI::UnitTypes::Protoss_Pylon)
		{
			FirstPylonExist = true;
		}
		else if (unit->getType() == BWAPI::UnitTypes::Protoss_Assimilator)
		{
			FirstGasExist = true;
		}
		else if (unit->getType() == BWAPI::UnitTypes::Protoss_Cybernetics_Core)
		{
			FirstCybernetics = true;
			PylonLimiter = 1;
		}
		else if (unit->getType() == BWAPI::UpgradeTypes::Singularity_Charge)
		{
			Singularity = true;
		}
		else if (unit->getType() == BWAPI::UnitTypes::Protoss_Citadel_of_Adun)
		{
			FirstAdun = true;
		}
		else if (unit->getType() == BWAPI::UpgradeTypes::Leg_Enhancements)
		{
			Leg_Enhancements = true;
		}

	}
}





void TurkAnalyzer::onUnitRenegade(BWAPI::Unit unit)
{
}

void TurkAnalyzer::onSaveGame(std::string gameName)
{
	Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}




/*
if (unit->getType() == BWAPI::UnitTypes::Protoss_Reaver)
{
unit->train(BWAPI::UnitTypes::Protoss_Scarab);
}
*/





/*


for (auto & unit : getBaseLocation()->getGeysers())
{
geyser = unit;
}

return geyser;

enemyGeyser->getPosition()


this->builder->buildAdditional(1, BWAPI::Broodwar->self()->getRace().getRefinery(), refineryBuildPriority, (*b)->getBaseLocation()->getTilePosition());

getType().isRefinery()

BWAPI::Broodwar->self()->getRace().getRefinery()
BWAPI::Game::getGeysers()


BWAPI::Unit ScoutManager::getEnemyGeyser()
{
BWAPI::Unit geyser = nullptr;
BWTA::BaseLocation * enemyBaseLocation = InformationManager::Instance().getMainBaseLocation(BWAPI::Broodwar->enemy());

for (auto & unit : enemyBaseLocation->getGeysers())
{
geyser = unit;
}

return geyser;
}


for (auto &u : Broodwar->getGeysers())
{
supplyBuilder->build(supplyProviderType, targetBuildLocation);
}



*/








