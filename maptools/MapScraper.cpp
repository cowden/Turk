#include "MapScraper.h"

#include <iostream>
#include <fstream>


using namespace BWAPI;
using namespace MapTools;


//
// default constructor
MapScraper::MapScraper() {

	// connect to configuration

	// 
	std::string tdir(std::getenv("TURKDIR"));
	m_baseName = tdir + std::string("\\maps\\");
}



void MapScraper::onStart()
{

	// enable complete map information
	Broodwar->enableFlag(Flag::CompleteMapInformation);


	bool map_exists = false;
	// check for map in output directory
	std::string mapName = Broodwar->mapFileName();
	std::string mapHash = Broodwar->mapHash();


	std::string oName;
	oName = m_baseName + mapName + std::string("_walkable.ppm");
	Broodwar->sendText("Base Name: %s",m_baseName);
	Broodwar->sendText("Creating map: %s", oName.c_str());

	//Broodwar->pauseGame();

	if (!map_exists) {
		// get map dimensions
		const int width = 4*Broodwar->mapWidth();
		const int height = 4*Broodwar->mapHeight();
		

		// create walkable map
		std::vector<unsigned> wmap(width*height);

		// explore pixels
		for (unsigned i = 0; i != height; i++) {
			for (unsigned j = 0; j != width; j++) {
				wmap[i*width + j] = (unsigned)Broodwar->isWalkable(j, i);
			}
		}

		// dump map
		FILE * file = fopen(oName.c_str(), "wb");
		fprintf(file, "P6\n%i %i 255\n", width, height);
		for (unsigned i = 0; i != height; i++) {
			for (unsigned j = 0; j != width; j++) {
				if (wmap[i*width + j]) {
					fputc(255, file);
					fputc(255, file);
					fputc(255, file);
				}
				else {
					fputc(0, file);
					fputc(0, file);
					fputc(0, file);
				}
			}
		}

		fclose(file);
	}

	// end game
	Broodwar->leaveGame();

}


void MapScraper::onEnd(bool isWinner)
{}


void MapScraper::onFrame()
{}


void MapScraper::onSendText(std::string text)
{}


void MapScraper::onReceiveText(BWAPI::Player player, std::string text)
{}


void MapScraper::onPlayerLeft(BWAPI::Player player)
{}


void MapScraper::onNukeDetect(BWAPI::Position target)
{}


void MapScraper::onUnitDiscover(BWAPI::Unit unit)
{}


void MapScraper::onUnitEvade(BWAPI::Unit unit)
{}


void MapScraper::onUnitShow(BWAPI::Unit unit)
{}


void MapScraper::onUnitHide(BWAPI::Unit unit)
{}


void MapScraper::onUnitCreate(BWAPI::Unit unit)
{}


void MapScraper::onUnitDestroy(BWAPI::Unit unit)
{}


void MapScraper::onUnitMorph(BWAPI::Unit unit)
{}


void MapScraper::onUnitRenegade(BWAPI::Unit unit)
{}


void MapScraper::onSaveGame(std::string gameName)
{}


void MapScraper::onUnitComplete(BWAPI::Unit unit)
{}


void MapScraper::drawTerrainData()
{}

