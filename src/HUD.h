#pragma once

/////////////////////////////////////////
// HUD - Heads Up Display
// For the visualization of AI and Game information.
/////////////////////////////////////////

#include <vector>
#include <sstream>
#include <string>

#include <BWAPI.h>

#include "bot.h"


namespace Turk {

	class HUD {

	public:

		static HUD & Instance();

		void drawInterface();

		/**
		* Allocate a new HUD lane.
		*/
		int getLane(const Turk::bot * b) {
			lane_names_[lanes_] = b->name();
			return lanes_++;
		}

		/**
		* Write to the stream.
		*/
		void write(int ln, const std::string & str) {
			lane_stream_[ln] << str << "\n";
		}

		/**
		* clear the lane contents
		*/
		void clear(int ln) {
			lane_stream_[ln].clear();
			lane_stream_[ln].str("");
		}
		

	private:

		HUD() :lanes_(0), lane_stream_(10),lane_names_(10) { }

		int lanes_;

		std::vector<std::stringstream> lane_stream_;
		std::vector<std::string> lane_names_;

	};
	 
} // end Turk namespace