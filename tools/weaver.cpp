
#include "weaver.h"

#include <cassert>

#include "ConfigDB.h"

using namespace Turk;


void Weaver::loadTechTree() {

	// connect to configuration database
	Turk::ConfigDB *db = Turk::ConfigDB::instance();

	// read the tech tree graph table
	int nrows = 0;
	std::vector<std::vector<char *> > vres = db->query(nrows,"SELECT * FROM techtree;");

	// parse results into data
	// we are expected a large square matrix of 0's and 1's
	// assert it is a square matrix
	assert(nrows == vres.size());
	assert(nrows == vres[0].size());

	tech_size_ = nrows;

	// resize the data matrix
	tech_tree_.resize(tech_size_*tech_size_);

	for (unsigned i = 0; i != nrows; i++) {
		for (unsigned j = 0; j != nrows; j++) {
			tech_tree_[i*nrows + j] = atoi(vres[i][j]);
		}
	}


}

Turk::Weaver Turk::weaver;