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
#include <limits>

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
	ArtieInterface() :start_(UINT_MAX), enemy_start_(UINT_MAX) { }

	/**
	* load artie object from file
	*/
	inline virtual void load_artie(const std::string & name) {
		std::ifstream artie_ar(name);
		boost::archive::text_iarchive ar(artie_ar);
		ar >> artie_;

		// prepare BWAPI game specific data arrays
		const unsigned N = artie_.get_chokes().size();

		control_.resize(N);
		occupy_.resize(N);
		enemy_control_.resize(N);
		enemy_occupy_.resize(N);
		
	}

	// ------------------------------
	// accessor methods

	

	/**
	* Return the region information given the walk tile (8x8 pixels)
	* location.
	*/
	inline virtual const inline region & get_region(unsigned x, unsigned y) const { return artie_.get_region(x,y); }

	/**
	* Return the region information given the walk tile by BWAPI::WalkPosition
	*/
	inline virtual const inline region & get_region(const BWAPI::WalkPosition & pos) const { return artie_.get_region(pos.x, pos.y); }

	/**
	* return a region by index
	*/
	inline virtual const region & operator[](unsigned i) const { return artie_[i]; }

	/**
	* get the map width
	*/
	inline virtual unsigned width() const { return artie_.width(); }

	/**
	* get the map height
	*/
	inline virtual unsigned height() const { return artie_.height(); }

	/**
	* return the distance map
	*/
	inline virtual const std::vector<unsigned> & get_distance_map() const { return artie_.get_distance_map(); }

	/**
	* return an ARTIE reference
	*/
	const inline virtual ARTIE & getARTIE() const { return artie_; }

	/**
	* Get Start/Base location
	*/
	inline virtual unsigned start_region() const { return start_; }

	/**
	* Return the initial start position (WalkPosition)
	*/
	inline virtual const BWAPI::WalkPosition & get_start_pos() const { return loc_; }

	/**
	* Get start locations.  This is merely a wrapper to the 
	* BWAPI function.
	*/
	inline virtual const BWAPI::TilePosition::list & get_start_positions() const { return BWAPI::Broodwar->getStartLocations();  }

	/**
	* Check bot occupies a region
	*/
	inline virtual unsigned occupies(unsigned i) const { return occupy_[i]; }

	/**
	* Check bot occupies a region (by BWAPI::WalkPosition
	*/
	inline virtual unsigned occupies(const BWAPI::WalkPosition & pos) const
	{
		return occupy_[get_region(pos).index()];
	}

	/**
	* Check if bot controls a region
	*/
	inline virtual unsigned controls(unsigned i) const { return control_[i]; }

	/**
	* Check if bot controls a region
	*/
	inline virtual unsigned controls(const BWAPI::WalkPosition & pos) const
	{
		return control_[get_region(pos).index()];
	}

	/**
	* Return the region index of the enemy start position
	*/
	inline virtual unsigned enemy_start() const { return enemy_start_; }

	/**
	* Check if enemy controls a region
	*/
	inline virtual unsigned enemy_controls(unsigned i) const { return enemy_control_[i]; }

	/**
	* Check if enemy controls a region
	*/
	inline virtual unsigned enemy_controls(const BWAPI::WalkPosition & pos) const
	{
		return enemy_control_[get_region(pos).index()];
	}

	/**
	* Check if enemy occupies a region
	*/
	inline virtual unsigned enemy_occupies(unsigned i) const { return enemy_occupy_[i]; }

	/**
	* Check if enemy occupies a region
	*/
	inline virtual unsigned enemy_occupies(const BWAPI::WalkPosition & pos) const
	{
		return enemy_occupy_[get_region(pos).index()];
	}

	// ------------------
	// update methods

	/**
	* set the start region
	*/
	inline virtual void set_start(unsigned indx) { start_ = indx; }

	/**
	* set the start position and region
	*/
	inline virtual void set_start(const BWAPI::WalkPosition & pos) {
		loc_ = pos;
		start_ = get_region(pos).index();
	}

	/**
	* occupy/unocupy region
	*/
	inline virtual void set_occupy(unsigned indx, unsigned o) { occupy_[indx] = o; }

	/**
	* control/lose control of region
	*/
	inline virtual void set_control(unsigned indx, unsigned c) { control_[indx] = c; }

	/**
	* enemy occupy/unoccupy region
	*/
	inline virtual void set_enemy_occupy(unsigned indx, unsigned o) { enemy_occupy_[indx] = o; }

	/**
	* enemy control/lose control of region
	*/
	inline virtual void set_enemy_control(unsigned indx, unsigned c) { enemy_control_[indx] = c; }

	/**
	* enemy base location
	*/
	inline virtual void set_enemy_start(unsigned indx) { enemy_start_ = indx; }

	
private:

	ARTIE artie_;

	// index of start/initial region
	unsigned start_;
	unsigned enemy_start_;

	// position of start (where is the initial command center)
	BWAPI::WalkPosition loc_;

	// indicator flags for control and occupation
	std::vector<unsigned> control_;
	std::vector<unsigned> occupy_;
	std::vector<unsigned> enemy_control_;
	std::vector<unsigned> enemy_occupy_;

};


} // end Turk namespace