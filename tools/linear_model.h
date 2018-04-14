#ifndef LINEAR_MODEL_H
#define LINEAR_MODEL_H

#include <vector>

namespace Turk {

/**
* A general class to implement 
* linear models. OLS
*/
class linear_model {

public:

  /**
  * default constructor
  */
  linear_model():been_fit(false) { }

  /**
  * Fit the model to the data.
  */
  void fit(double * x, double * y, const unsigned n_features, const unsigned n_obs);

  // ---
  // accessor methods
  const std::vector<double> & coefficients() const { return m_coefs; }

  /**
  * Return fit status
  */
  inline bool success() const {
    if ( !been_fit ) return false;

    return m_info == 0;
  }

  /**
  * return the fit info flag
  */
  inline const int fit_info() const { return m_info; }

protected:

  // the model coefficients
  std::vector<double> m_coefs;

  // result
  bool been_fit;

private:

  int m_info;
  
};

} // end Turk namespace

#endif
