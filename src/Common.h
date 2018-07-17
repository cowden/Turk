#pragma once

// #include "BWAPI.h"


#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <array>

#include <BWAPI.h>
#include <windows.h>

#include "weaver.h"
#include "artie.h"
#include "cameron.h"


// Basic Building
namespace Turk {

/**
* Convenience to store locations
*/
struct location {
	int x;
	int y;

	location(int tx, int ty) :x(tx), y(ty) { }
	location():x(-1),y(-1) { }
};

/**
* elevate status for IRQ handling
*/
enum status {
	GOOD,
	WARN,
	ATTN,
	URGNT
};

} // end Turk namespace