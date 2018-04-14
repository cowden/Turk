

#include "dbscan.h"

using namespace Turk;


void dbscan::fit(const double * data, const unsigned n_features, const unsigned n_obs) {

  // cycle over points in data
  for ( unsigned i=0; i != n_obs; i++ ) {

    // if already labelled continue

    // find neighbors

    // density check => label as noise and continue

    // increment cluster label
    // label initial point
    // process seed points
      // change noise to border point
      // if previously processes, continue
      // collect neighbors
      // check density, if above add new points to queue

  }

}
