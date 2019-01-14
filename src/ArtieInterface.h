#pragma once

/////////////////////////////////////////
// ArtieInterface
// C S Cowden      9 January 2019
// A BWAPI specific wrapper to interface
// with ARITE.
//////////////////////////////////////////


#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>

#include "BWAPI.h"

#include "Common.h"
#include "artie.h"

namespace Turk {

/**
* ArtieInterface is the BWAPI specific interface class
* between the Turk bot elements and ARTIE map representation.
*/
class ArtieInterface {
public:

	/**
	* Default constructor
	*/

	/**
	* load artie object from file
	*/
	inline virtual void load_artie(const std::string & name) {
		std::ifstream artie_ar(name);
		boost::archive::text_iarchive ar(artie_ar);
		ar >> artie_;
	}

	// ------------------------------
	// accessor methods

	/**
	* Return the region information given the walk tile (8x8 pixels)
	* location.
	*/

	/**
	* Return the region information given the walk tile by BWAPI::WalkPosition
	*/

	/**
	* return a region by index
	*/

	/**
	* get the map width
	*/

	/**
	* get the map height
	*/

	/**
	* return the distance map
	*/

	/**
	* return an ARTIE reference
	*/
	const inline ARTIE & getARTIE() const { return artie_; }

	/**
	* Get Start/Base location
	*/

	/**
	* Get start locations
	*/

	/**
	* Check bot occupies a region
	*/

	/**
	* Check if bot controls a region
	*/

	/**
	* Check if enemy occupies a region
	*/

	/**
	* Check if enemy controls a region
	*/

	/**
	* Return the enemy base location
	*/


	// ------------------
	// update methods

	/**
	* occupy/unocupy region
	*/

	/**
	* control/lose control of region
	*/

	/**
	* enemy occupy/unoccupy region
	*/

	/**
	* enemy control/lose control of region
	*/

	/**
	* enemy base location
	*/

	
private:

	ARTIE artie_;

};
} // end Turk namespace