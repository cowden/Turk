

#include "dbscan.h"

#include <cassert>


using namespace Turk;


void dbscan::fit(const double * data, const unsigned n_features, const unsigned n_obs) {

  // workspace to store neighbors
  std::vector<unsigned> neighbs(n_obs);
  std::vector<int> queue(n_obs*n_obs);

  // construct the kdtree
  kdt_ = kdtree(data,n_features,n_obs);

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
    const unsigned N = rangeSearch(i,&neighbs[0],data,n_features,n_obs);
    

    // density check => label as noise and continue
    if ( N < min_samples_ ) {
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
    unsigned ql = N; // length of the queue

    while ( ql > 0 ) {

      // take one out of the queue
      const unsigned index = queue[--ql];

      // if previously processes, continue
      if ( labels_[index] > 0 ) continue;

      // change noise to border point
      if ( labels_[index] <= 0 ) labels_[index] = cluster_label;

      // collect neighbors
      const unsigned n = rangeSearch(index,&neighbs[0],data,n_features,n_obs);


      // check density, if above add new points to queue
      if ( n > min_samples_ ) {
        for ( unsigned j=0; j != n; j++ ) 
          queue[ql+j] = neighbs[j];
        ql += n;
      }
    }

  }

}




unsigned dbscan::rangeSearch(unsigned index, unsigned * workspace, const double * data, unsigned n_features, const unsigned n_obs) {

  
  kdpoint pt(n_features,&data[index*n_features]); 
  
  const unsigned nn = kdt_.ballsearch(pt,eps_,workspace,nnmax_);
  //const unsigned nn = 0;

  return nn;
}
