#ifndef DBSCAN_H
#define DBSCAN_H

///////////////////////////////////
// C S Cowden   12 April 2018
// DBSCAN implementation for use in
// ARTIE terrain analysis.
///////////////////////////////////

#include <vector>

namespace Turk  {

class dbscan {

public:
 
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
  virtual unsigned rangeSearch(int * workspace, const double * data, const unsigned n_features, const unsigned n_obs);

  std::vector<int> labels_;

  double eps_;

};

} // end Turk namespace


#endif
