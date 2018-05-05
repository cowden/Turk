

#include "dbscan.h"

#include <cassert>

#include "kdtree.h"

using namespace Turk;


void dbscan::fit(const double * data, const unsigned n_features, const unsigned n_obs) {

  // workspace to store neighbors
  std::vector<int> neighbs(n_obs);
  std::vector<int> queue(n_obs*n_obs);

  // cluster labels:
  // -1 = unlabeled
  // 0 = noise
  // 1+ = a cluster 
  int cluster_label = 0;

  // initialize cluster labels to -1
  labels_.resize(n_obs);
  for ( unsigned i=0; i != n_obs; i++ )
    labels_[i] = -1;


  // cycle over points in data
  for ( unsigned i=0; i != n_obs; i++ ) {

    // if already labelled continue
    if ( labels_[i] != -1 ) continue;

    // find neighbors
    const unsigned N = rangeSearch(&neighbs[0],data,n_features,n_obs);
    

    // density check => label as noise and continue
    if ( N < eps_ ) {
      labels_[i] = 0;
      continue;
    }

    // increment cluster label
    cluster_label++;

    // label initial point
    labels_[i] = cluster_label;

    // fill queue
    for ( unsigned j=0; j != N; j++ )
      queue[j] = neighbs[j];

    // process seed points
    unsigned loc = 0;
    unsigned ql = N; // length of the queue

    while ( ql > 0 ) {

      // take one out of the queue
      ql--;

      const unsigned index = neighbs[loc];

      // change noise to border point
      if ( labels_[index] == 0 ) labels_[index] = cluster_label;

      // if previously processes, continue
      if ( labels_[index] > 0 ) continue;

      // collect neighbors
      const unsigned n = rangeSearch(&neighbs[0],data,n_features,n_obs);


      // check density, if above add new points to queue
      if ( n < eps_ ) {
        for ( unsigned j=0; j != n; j++ ) 
          queue[ql+j] = neighbs[j];
        ql += n;
      }
    }

  }

}




unsigned dbscan::rangeSearch(int * workspace, const double * data, const unsigned n_features, const unsigned n_obs) {

  kdtree kdt(data,n_features,n_obs);

  return 0;
}
