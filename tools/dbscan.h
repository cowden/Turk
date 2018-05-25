#ifndef DBSCAN_H
#define DBSCAN_H

///////////////////////////////////
// C S Cowden   12 April 2018
// DBSCAN implementation for use in
// ARTIE terrain analysis.
///////////////////////////////////

#include <vector>

#include "kdtree.h"


namespace Turk  {


class dbscan {

public:

  /**
  * default constructor
  */
  dbscan(): min_samples_(1),eps_(10.),nnmax_(100) { }
 
  /**
  * Execute DBSCAN to  find clusters
  * from the given points.
  */ 
  virtual void fit(const double * data,const unsigned n_features, const unsigned n_obs);



  // ---------
  // accessor methods

  /**
  * return the list of labels for each input point.
  */
  virtual const std::vector<int> & getLabels() const { return labels_; }

 
private:

  /**
  * Find neighbors with in given range of a point.
  */
  virtual unsigned rangeSearch(unsigned index, unsigned * workspace, const double * data, const unsigned n_features, const unsigned n_obs);

  std::vector<int> labels_;

  double min_samples_;
  double eps_;
  unsigned nnmax_;

  // kdtree data
  kdtree kdt_;

};

} // end Turk namespace


#endif
