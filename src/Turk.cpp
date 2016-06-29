#include "Turk.h"
#include <iostream>

bool analyzed;
bool analysis_just_finished;

using namespace BWAPI;
using namespace Filter;
using namespace Turk;





void TheTurk::onStart()
{
	// Hello World!
	// Broodwar->sendText("Hello world!");
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
	if (Broodwar->isReplay()){
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


	// BWTA Map Analysis
	BWTA::readMap();
	BWTA::analyze();
	analyzed = true;
	analysis_just_finished = true;






	// Variable Definition
	homePosition = BWAPI::Position(Broodwar->self()->getStartLocation());
	homeTilePosition = Broodwar->self()->getStartLocation();
	HillPosition = BWTA::getNearestChokepoint(BWAPI::Position(Broodwar->self()->getStartLocation()))->getCenter();




	// Define the location of the first pylon
	Broodwar->sendText("Pylon %.2d:%.2d", FirstPylonTilePosition.x, FirstPylonTilePosition.y);

	

	if ( (abs(homePosition.x - HillPosition.x) > abs(homePosition.y - HillPosition.y)) && (homePosition.x - HillPosition.x > 0)){
		// Probably 12
		
		// Location of the first pylon
		BWAPI::Position Temp;
		Temp.x = (PylonToChoke * homePosition.x + BaseToPylon* HillPosition.x) / (BaseToPylon + PylonToChoke);
		Temp.y = (PylonToChoke * homePosition.y + BaseToPylon* HillPosition.y) / (BaseToPylon + PylonToChoke);
		FirstPylonTilePosition = BWAPI::TilePosition(Temp);

		SecondPylonTilePosition.x = FirstPylonTilePosition.x;
		SecondPylonTilePosition.y = FirstPylonTilePosition.y + Pylon.tileHeight();

		ThirdPylonTilePosition.x = FirstPylonTilePosition.x;
		ThirdPylonTilePosition.y = FirstPylonTilePosition.y - Pylon.tileHeight();


		FirstGateLocation.x = FirstPylonTilePosition.x - Pylon.tileWidth();
		FirstGateLocation.y = FirstPylonTilePosition.y;

		SecondGateLocation.x = FirstGateLocation.x;
		SecondGateLocation.y = FirstGateLocation.y + GateWay.tileHeight();

		ThirdGateLocation.x = FirstGateLocation.x;
		ThirdGateLocation.y = FirstGateLocation.y - GateWay.tileHeight();

		

	}
	else if ((abs(homePosition.x - HillPosition.x) > abs(homePosition.y - HillPosition.y)) && (homePosition.x - HillPosition.x < 0)){
		// Probably 6

		// Location of the first pylon
		BWAPI::Position Temp;
		Temp.x = (PylonToChoke * homePosition.x + BaseToPylon* HillPosition.x) / (BaseToPylon + PylonToChoke);
		Temp.y = (PylonToChoke * homePosition.y + BaseToPylon* HillPosition.y) / (BaseToPylon + PylonToChoke);
		FirstPylonTilePosition = BWAPI::TilePosition(Temp);

		SecondPylonTilePosition.x = FirstPylonTilePosition.x;
		SecondPylonTilePosition.y = FirstPylonTilePosition.y + Pylon.tileHeight();

		ThirdPylonTilePosition.x = FirstPylonTilePosition.x;
		ThirdPylonTilePosition.y = FirstPylonTilePosition.y - Pylon.tileHeight();


		// Build toward the choke
		FirstGateLocation.x = FirstPylonTilePosition.x + Pylon.tileWidth();
		FirstGateLocation.y = FirstPylonTilePosition.y;

		SecondGateLocation.x = FirstGateLocation.x;
		SecondGateLocation.y = FirstGateLocation.y + GateWay.tileHeight();

		ThirdGateLocation.x = FirstGateLocation.x;
		ThirdGateLocation.y = FirstGateLocation.y - GateWay.tileHeight();

	}
	else if ((abs(homePosition.x - HillPosition.x) < abs(homePosition.y - HillPosition.y)) && (homePosition.y - HillPosition.y > 0)){
		// Probably 9
		FirstPylonTilePosition.x = homeTilePosition.x + Nexus.tileWidth() + 2;
		FirstPylonTilePosition.y = homeTilePosition.y;

		SecondPylonTilePosition.x = FirstPylonTilePosition.x;
		SecondPylonTilePosition.y = FirstPylonTilePosition.y + Pylon.tileHeight();

		ThirdPylonTilePosition.x = FirstPylonTilePosition.x;
		ThirdPylonTilePosition.y = FirstPylonTilePosition.y - Pylon.tileHeight();


		FirstGateLocation.x = FirstPylonTilePosition.x + Pylon.tileWidth();
		FirstGateLocation.y = FirstPylonTilePosition.y;

		SecondGateLocation.x = FirstGateLocation.x;
		SecondGateLocation.y = FirstGateLocation.y + GateWay.tileHeight();

		ThirdGateLocation.x = FirstGateLocation.x;
		ThirdGateLocation.y = FirstGateLocation.y - GateWay.tileHeight();



	}
	else if ((abs(homePosition.x - HillPosition.x) < abs(homePosition.y - HillPosition.y)) && (homePosition.y - HillPosition.y < 0)){
		// Probably 3

		// Location of the first pylon
		BWAPI::Position Temp;
		Temp.x = (PylonToChoke * homePosition.x + BaseToPylon* HillPosition.x) / (BaseToPylon + PylonToChoke);
		Temp.y = (PylonToChoke * homePosition.y + BaseToPylon* HillPosition.y) / (BaseToPylon + PylonToChoke);
		FirstPylonTilePosition = BWAPI::TilePosition(Temp);

		SecondPylonTilePosition.x = FirstPylonTilePosition.x + Pylon.tileWidth();
		SecondPylonTilePosition.y = FirstPylonTilePosition.y;

		ThirdPylonTilePosition.x = FirstPylonTilePosition.x - Pylon.tileWidth();
		ThirdPylonTilePosition.y = FirstPylonTilePosition.y;
		
		FirstGateLocation.x = FirstPylonTilePosition.x;
		FirstGateLocation.y = FirstPylonTilePosition.y + Pylon.tileHeight();

		SecondGateLocation.x = FirstGateLocation.x + GateWay.tileWidth();
		SecondGateLocation.y = FirstGateLocation.y ;

		ThirdGateLocation.x = FirstGateLocation.x - GateWay.tileWidth();
		ThirdGateLocation.y = FirstGateLocation.y;

	}
	
	PylonTilePosition.push_back(FirstPylonTilePosition);
	PylonTilePosition.push_back(SecondPylonTilePosition);
	PylonTilePosition.push_back(ThirdPylonTilePosition);

	
	GateTilePosition.push_back(FirstGateLocation);
	GateTilePosition.push_back(SecondGateLocation);
	GateTilePosition.push_back(ThirdGateLocation);






	// A set of minerals
	BWTA::BaseLocation* StartingPoint = BWTA::getStartLocation(BWAPI::Broodwar->self());
	BWAPI::Unitset FirstMineralSet = _Commander.MineralCollector(StartingPoint); // 

	// Very Basic Initiation: Split workers optimally
	for (auto &unit : Broodwar->self()->getUnits()){
		// A resource depot is a Command Center, Nexus, or Hatchery
		if (unit->getType().isResourceDepot()){
			ResourceDepot = unit;
			// Generate the first worker
			unit->train(unit->getType().getRace().getWorker());
		}
		// If the unit is a worker unit
		else if (unit->getType().isWorker()) {
			BWAPI::Unit ClosestMineral = nullptr;
			double closestDist = 100000;
			// For the given initial Minerals, find the closest mineral one by one
			for (auto &mineral : FirstMineralSet){
				double dist = unit->getDistance(mineral->getPosition());
				//Broodwar->sendText("%.2d / Unit: %s", dist, unit->getType().c_str());
				if (dist < closestDist){
					ClosestMineral = mineral;
					closestDist = dist;
				}
			}
			unit->gather(ClosestMineral);
			FirstMineralSet.erase(ClosestMineral);
		}
	}
	_Commander.MineralSaver(FirstMineralSet);


















}

















// Display the game frame rate as text in the upper left area of the screen
//Broodwar->drawTextScreen(200, 0,  "FPS: %d", Broodwar->getFPS() );
//Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS() );

void TheTurk::onFrame(){
	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	//BWTA draw
	if (analyzed){
		drawTerrainData();
	}
	
	if (analysis_just_finished)	{
		Broodwar << "Finished analyzing map." << std::endl;;
		analysis_just_finished = false;
	}


	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	// ------------------------------------------------------------------------------------------------------------------------













	// Collect all possible units
	_Commander.ValidUnitCollector();
	
	// Count the number of probes
	std::map<std::string, int>	UnitCount;
	UnitCount = _Commander.UnitCounter();

	
	
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%	
	// Generate workers	
	_Commander.ProbeMaker(MaxWorkerCount);
			
	
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Work Work Work
	_Commander.ProbeWork(MaxMineralDist);
	















	// Build the First supply unit	
	if (BWAPI::Broodwar->self()->supplyUsed() / 2 >= 8 && UnitCount["Pylon_Count"]==0){
		static int lastChecked = 0;
		// If we are supply blocked and haven't tried constructing more recently
		if (lastChecked + 300 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(Pylon) == 0){
			lastChecked = Broodwar->getFrameCount();

			// Retrieve a unit that is capable of constructing the supply needed
			Unit supplyBuilder = ResourceDepot->getClosestUnit(GetType == Pylon.whatBuilds().first && (IsWorker)); // IsIdle || IsCarryingMinerals
			// If a unit was found
			if (supplyBuilder){
				TilePosition targetBuildLocation = Broodwar->getBuildLocation(Pylon, PylonTilePosition[0], 4); // supplyBuilder->getTilePosition()
						
				if (targetBuildLocation){
					// Order the builder to construct the supply structure	
					supplyBuilder->build(Pylon, targetBuildLocation);
					Scout = supplyBuilder;
				}
			}			
		}
	}




	// Build the supply unit
	Error lastErr = Broodwar->getLastError();
	if (UnitCount["Pylon_Count"] >= 1){
		if (BWAPI::Broodwar->self()->supplyUsed() / 2 + 4 > BWAPI::Broodwar->self()->supplyTotal() / 2){ // 
			
			static int lastChecked = 0;
			// If we are supply blocked and haven't tried constructing more recently
			if (lastChecked + 300 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(Pylon) == 0){
				lastChecked = Broodwar->getFrameCount();

				// Retrieve a unit that is capable of constructing the supply needed
				Unit supplyBuilder = ResourceDepot->getClosestUnit(GetType == Pylon.whatBuilds().first && (IsIdle || IsGatheringMinerals));
				// If a unit was found
				if (supplyBuilder){
					TilePosition targetBuildLocation = Broodwar->getBuildLocation(Pylon, PylonTilePosition[1],4);
					if (targetBuildLocation){
						// Order the builder to construct the supply structure				
						supplyBuilder->build(Pylon, targetBuildLocation);
					}
				}
			}
		}	
	}



	//Broodwar->sendText("Pylon :%.1d", UnitCount["Pylon_Count"]);



	// Build the first GateWay.
	if (UnitCount["Pylon_Count"] >= 1 && UnitCount["GateWay_Count"] == 0){
		
		static int lastChecked = 0;
		// If we are supply blocked and haven't tried constructing more recently
		if (lastChecked + 300 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(GateWay) == 0){
			lastChecked = Broodwar->getFrameCount();

			// Retrieve a unit that is capable of constructing the supply needed
			Unit supplyBuilder = ResourceDepot->getClosestUnit(GetType == GateWay.whatBuilds().first && (IsWorker));
			// If a unit was found
			if (supplyBuilder){
				TilePosition targetBuildLocation = Broodwar->getBuildLocation(GateWay, GateTilePosition[0], 4);

				if (targetBuildLocation){
					// Order the builder to construct the supply structure				
					supplyBuilder->build(GateWay, targetBuildLocation);
					Scout = supplyBuilder;
				}
			}
		}
	}
		


	// Build a Gas
	if (UnitCount["GateWay_Count"] >= 1 && !FirstGasExist){

		BWAPI::TilePosition closestGeyser = BWAPI::TilePositions::None;
		BWTA::BaseLocation *Base = BWTA::getNearestBaseLocation(homePosition);
	
		for (auto &gas : Base->getGeysers()){
			closestGeyser = gas->getInitialTilePosition();
		}
		static int lastChecked = 0;

		// If we are supply blocked and haven't tried constructing more recently
		if (lastChecked + 400 < Broodwar->getFrameCount() && Broodwar->self()->incompleteUnitCount(Assimilator) == 0){
			// Frame Count
			lastChecked = Broodwar->getFrameCount();

			// Retrieve a unit that is capable of constructing the supply needed				
			Unit GasBuilder = ResourceDepot->getClosestUnit(GetType == Assimilator.whatBuilds().first && (IsWorker));
			if (GasBuilder){
				TilePosition targetBuildLocation = Broodwar->getBuildLocation(Assimilator, closestGeyser, 4);

				if (targetBuildLocation){
					// Order the builder to construct the supply structure				
					GasBuilder->build(Assimilator, targetBuildLocation);
				}
			}
		}
	}






	// Send workers to the gas refinary for each base we have
	for (auto & unit : _Commander.BasePresent()){			
		BWAPI::Unit GasContainer = unit->getClosestUnit(IsRefinery && IsOwned && IsCompleted);
			
		// if that unit is a refinery
		if (GasContainer){
			int GasWorkerCount = 0;
			for (auto & unit2 : _Commander.WorkerPresent()){
				if (unit2->isGatheringGas() || unit2->isCarryingGas()){
					GasWorkerCount = GasWorkerCount + 1;
				}
			}

			if (GasWorkerCount >= WorkersPerGas){
				break;
			}
			else{
				int GasWorkerCount = 0;
				for (auto & unit2 : _Commander.WorkerPresent()){
					// Only send gathering workers to gas mine.
					if (unit2->isGatheringMinerals() && !unit2->isCarryingMinerals()){
						unit2->rightClick(GasContainer);
						GasWorkerCount = GasWorkerCount + 1;
						if (GasWorkerCount >= WorkersPerGas){
							break;
						}
					}					
				}
			}
		}
	}
	




















}









// Broodwar->sendText("Pylon :%.3u", WorkerSet.size());


//BWAPI::Position  FirstPylonTilePosition  = mineral->getPosition();
//Broodwar->sendText("Pylon :%.3f", dist);













//// Iterate through all the units that we own
//for (auto &u : Broodwar->self()->getUnits())
//{
//  // Ignore the unit if it no longer exists
//  // Make sure to include this block when handling any Unit pointer!
//  if ( !u->exists() )
//    continue;

//  // Ignore the unit if it has one of the following status ailments
//  if ( u->isLockedDown() || u->isMaelstrommed() || u->isStasised() )
//    continue;

//  // Ignore the unit if it is in one of the following states
//  if ( u->isLoaded() || !u->isPowered() || u->isStuck() )
//    continue;

//  // Ignore the unit if it is incomplete or busy constructing
//  if ( !u->isCompleted() || u->isConstructing() )
//    continue;
//

//  // Finally make the unit do some stuff!


//  // If the unit is a worker unit
//  if ( u->getType().isWorker() )
//  {
//    // if our worker is idle
//    if ( u->isIdle() )
//    {
//      // Order workers carrying a resource to return them to the center,
//      // otherwise find a mineral patch to harvest.
//      if ( u->isCarryingGas() || u->isCarryingMinerals() )
//      {
//        u->returnCargo();
//      }
//      else if ( !u->getPowerUp() )  // The worker cannot harvest anything if it
//      {                             // is carrying a powerup such as a flag
//        // Harvest from the nearest mineral patch or gas refinery
//        if ( !u->gather( u->getClosestUnit( IsMineralField || IsRefinery )) )
//        {
//          // If the call fails, then print the last error message
//          Broodwar << Broodwar->getLastError() << std::endl;
//        }

//      } // closure: has no powerup
//    } // closure: if idle

//  }
//  else if ( u->getType().isResourceDepot() ) // A resource depot is a Command Center, Nexus, or Hatchery
//  {

//    // Order the depot to construct more workers! But only when it is idle.
//    if ( u->isIdle() && !u->train(u->getType().getRace().getWorker()) )
//    {
//      // If that fails, draw the error at the location so that you can visibly see what went wrong!
//      // However, drawing the error once will only appear for a single frame
//      // so create an event that keeps it on the screen for some frames
//      Position pos = u->getPosition();
//      Error lastErr = Broodwar->getLastError();
//      Broodwar->registerEvent([pos,lastErr](Game*){ Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
//                              nullptr,    // condition
//                              Broodwar->getLatencyFrames());  // frames to run

//      // Retrieve the supply provider type in the case that we have run out of supplies
//      UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
//      static int lastChecked = 0;

//      // If we are supply blocked and haven't tried constructing more recently
//      if (  lastErr == Errors::Insufficient_Supply &&
//            lastChecked + 400 < Broodwar->getFrameCount() &&
//            Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0 )
//      {
//        lastChecked = Broodwar->getFrameCount();

//        // Retrieve a unit that is capable of constructing the supply needed
//        Unit supplyBuilder = u->getClosestUnit(  GetType == supplyProviderType.whatBuilds().first &&
//                                                  (IsIdle || IsGatheringMinerals) &&
//                                                  IsOwned);
//        // If a unit was found
//        if ( supplyBuilder )
//        {
//          if ( supplyProviderType.isBuilding() )
//          {
//            TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
//            if ( targetBuildLocation )
//            {
//              // Register an event that draws the target build location
//              Broodwar->registerEvent([targetBuildLocation,supplyProviderType](Game*)
//                                      {
//                                        Broodwar->drawBoxMap( Position(targetBuildLocation),
//                                                              Position(targetBuildLocation + supplyProviderType.tileSize()),
//                                                              Colors::Blue);
//                                      },
//                                      nullptr,  // condition
//                                      supplyProviderType.buildTime() + 100 );  // frames to run

//              // Order the builder to construct the supply structure
//              supplyBuilder->build( supplyProviderType, targetBuildLocation );
//            }
//          }
//          else
//          {
//            // Train the supply provider (Overlord) if the provider is not a structure
//            supplyBuilder->train( supplyProviderType );
//          }
//        } // closure: supplyBuilder is valid
//      } // closure: insufficient supply
//    } // closure: failed to train idle unit

//  }

//} // closure: unit iterator















DWORD WINAPI AnalyzeThread()
{
	BWTA::analyze();

	analyzed = true;
	analysis_just_finished = true;
	return 0;
}




void TheTurk::drawTerrainData()
{
	//we will iterate through all the base locations, and draw their outlines.
	for (const auto& baseLocation : BWTA::getBaseLocations()) {
		TilePosition p = baseLocation->getTilePosition();

		//draw outline of center location
		Position leftTop(p.x * TILE_SIZE, p.y * TILE_SIZE);
		Position rightBottom(leftTop.x + 4 * TILE_SIZE, leftTop.y + 3 * TILE_SIZE);
		Broodwar->drawBoxMap(leftTop, rightBottom, Colors::Blue);

		//draw a circle at each mineral patch
		for (const auto& mineral : baseLocation->getStaticMinerals()) {
			Broodwar->drawCircleMap(mineral->getInitialPosition(), 30, Colors::Cyan);
		}

		//draw the outlines of Vespene geysers
		for (const auto& geyser : baseLocation->getGeysers()) {
			TilePosition p1 = geyser->getInitialTilePosition();
			Position leftTop1(p1.x * TILE_SIZE, p1.y * TILE_SIZE);
			Position rightBottom1(leftTop1.x + 4 * TILE_SIZE, leftTop1.y + 2 * TILE_SIZE);
			Broodwar->drawBoxMap(leftTop1, rightBottom1, Colors::Orange);
		}

		//if this is an island expansion, draw a yellow circle around the base location
		if (baseLocation->isIsland()) {
			Broodwar->drawCircleMap(baseLocation->getPosition(), 80, Colors::Yellow);
		}
	}

	//we will iterate through all the regions and ...
	for (const auto& region : BWTA::getRegions()) {
		// draw the polygon outline of it in green
		BWTA::Polygon p = region->getPolygon();
		for (size_t j = 0; j < p.size(); ++j) {
			Position point1 = p[j];
			Position point2 = p[(j + 1) % p.size()];
			Broodwar->drawLineMap(point1, point2, Colors::Green);
		}
		// visualize the chokepoints with red lines
		for (auto const& chokepoint : region->getChokepoints()) {
			Position point1 = chokepoint->getSides().first;
			Position point2 = chokepoint->getSides().second;
			Broodwar->drawLineMap(point1, point2, Colors::Red);
		}
	}
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
	//if ( Broodwar->isReplay() )
	//{
	//  // if we are in a replay, then we will print out the build order of the structures
	//  if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
	//  {
	//    int seconds = Broodwar->getFrameCount()/24;
	//    int minutes = seconds/60;
	//    seconds %= 60;
	//    Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
	//  }
	//}
}

void TheTurk::onUnitDestroy(BWAPI::Unit unit)
{
	//BWAPI::Unitset Base = _Commander.BasePresent();
	//Base.erase(unit);
	//_Commander.MineralSaver(Base);
}

void TheTurk::onUnitMorph(BWAPI::Unit unit)
{
	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount() / 24;
			int minutes = seconds / 60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
	}
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
	if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
	{
		// Position pos = unit->getPosition();
		//int seconds = Broodwar->getFrameCount() / 24;
		//int minutes = seconds / 60;
		//seconds %= 60;
		// Broodwar->sendText("%.2d:%.2d: %s completed a %s", pos, unit->getPlayer()->getName().c_str(), unit->getType().c_str());

		if (unit->getType() == BWAPI::UnitTypes::Protoss_Assimilator)
		{
			FirstGasExist = true;
		}
	}


}












void TheTurk::onEnd(bool isWinner)
{
	// Called when the game ends
	if (isWinner)
	{
		// Log your win here!
	}
}





//int d=0;
//for (auto &mineral : FirstMineralSet){
//	d = d + 1;
//	BWAPI::Position targetBuildLocation = mineral->getPosition();
//	BWAPI::Broodwar->sendText("%.2d %.2d / Unit: %s, %d", targetBuildLocation.x, targetBuildLocation.y, mineral->getType().c_str(), d);
//}



//// Very Basic Initiation
//for (auto &unit : Broodwar->self()->getUnits()){
//	// A resource depot is a Command Center, Nexus, or Hatchery
//	if (unit->getType().isResourceDepot()){
//		// Generate the first worker
//		unit->train(unit->getType().getRace().getWorker());
//	}
//	// If the unit is a worker unit
//	else if (unit->getType().isWorker()) {
//		BWAPI::Unit ClosestMineral = nullptr;
//		double closestDist = 100000;

//		// For the given initial Minerals, find the closest mineral one by one
//		for (auto &mineral : Minerals){
//			double dist = unit->getDistance(mineral->getPosition());
//			if (dist < closestDist){
//				ClosestMineral = mineral;
//				closestDist = dist;
//			}
//			unit->gather(ClosestMineral);
//			Minerals.erase(ClosestMineral);
//		}
//	}
//}






//// Go to work and Generate Probe
//for (auto &unit : _Commander.UnitSetPresent()){   //  Broodwar->self()->getUnits()
//	// If the unit is a worker unit
//	if (unit->getType().isWorker()) {


//		// if our worker is idle
//		if (unit->isIdle()) {
//			// Order workers carrying a resource to return them to the center		     
//			if (unit->isCarryingGas() || unit->isCarryingMinerals()){
//				unit->returnCargo();
//			}
//			// otherwise find a mineral patch to harvest.					
//			else{
//				BWAPI::Unit ClosestMineral = nullptr;
//				double closestDist = 100000;

//				// For the given initial Minerals, find the closest mineral one by one					
//				for (auto &mineral : FirstMineralSet){
//					double dist = unit->getDistance(mineral->getPosition());
//					if (dist < closestDist){
//						ClosestMineral = mineral;
//						closestDist = dist;
//					}
//				}
//				unit->gather(ClosestMineral);
//				FirstMineralSet.erase(ClosestMineral);
//				_Commander.MineralSaver(FirstMineralSet);
//			}
//		}
//	}
//	
//	else if (unit->getType().isResourceDepot()){
//		// Order the depot to construct more workers! But only when it is idle.
//		if (unit->isIdle()){
//			// Train a worker
//			unit->train(unit->getType().getRace().getWorker());
//		}
//	}
//}




//// Python Map
//// 12
//if (homePosition.y < 500)
//{
//	StartClockPosition = 12;

//	

//}
//// 3
//else if (homePosition.x > 3000)
//{
//	StartClockPosition = 3;


//	FirstPylonLocation.x = homeTilePosition.x;
//	FirstPylonLocation.y = homeTilePosition.y+2;

//}
//// 6
//else if (homePosition.y > 3000)
//{
//	StartClockPosition = 6;
//}
//// 9
//else if (homePosition.x < 500)
//{
//	StartClockPosition = 9;

//}




//if (FirstGateExist && !FirstGasExist){

//	BWAPI::TilePosition closestGeyser = BWAPI::TilePositions::None;
//	double minGeyserDistanceFromHome = std::numeric_limits<double>::max();		

//	// for each geyser
//	for (auto & geyser : Broodwar->getStaticGeysers()){
//		if (geyser->getType() != BWAPI::UnitTypes::Resource_Vespene_Geyser){
//			continue;
//		}

//		BWAPI::Position geyserPos = geyser->getInitialPosition();
//		
//		// check to see if it's next to one of our depots
//		bool nearDepot = false;
//		for (auto & unit : BWAPI::Broodwar->self()->getUnits()){
//			if (unit->getType().isResourceDepot() && unit->getDistance(geyserPos) < 300){
//				nearDepot = true;
//			}
//		}

//		if (nearDepot){
//			double homeDistance = geyser->getDistance(homePosition);

//			if (homeDistance < minGeyserDistanceFromHome){
//				minGeyserDistanceFromHome = homeDistance;
//				closestGeyser = geyser->getInitialTilePosition();
//			}
//		}


//	
//	}
//}
