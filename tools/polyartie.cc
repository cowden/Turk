
#include <cmath>

#include "polyartie.h"

using namespace Turk;

polyartie::polyartie(const std::vector<double> & x
  ,const std::vector<double> & y
  ,const std::vector<double> & depth )
{

  // prepare the A matrix
  // [x^2 xy y^2 x y 1]
  const unsigned n_features = 6;
  const unsigned n_obs = x.size();
  std::vector<double> A(n_features*n_obs);
  std::vector<double> b(n_obs);
  
  for ( unsigned i=0; i != n_obs; i++ ) {
    A[i*n_features] = x[i]*x[i];
    A[i*n_features+1] = x[i]*y[i];
    A[i*n_features+2] = y[i]*y[i];
    A[i*n_features+3] = x[i];
    A[i*n_features+4] = y[i];
    A[i*n_features+5] = 1.;

    b[i] = depth[i];
  }

  fit(&A[0],&b[0],n_features,n_obs);

}


bool polyartie::isMinima() {

  if ( been_fit && success() ) {

    calcMinima();

    if ( fabs(m_min.first) <= 0.5 && fabs(m_min.second) <= 0.5 )
      return true;
  }

  return false;
}

void polyartie::calcMinima() {

  if ( been_fit ) {

    const double denom = 4*m_coefs[0]*m_coefs[2]-m_coefs[1]*m_coefs[1];
    
    const double xmin = (m_coefs[1]*m_coefs[4]-2.*m_coefs[2]*m_coefs[3])/denom;
    const double ymin = (m_coefs[1]*m_coefs[3]-2.*m_coefs[0]*m_coefs[4])/denom;

    m_min = std::pair<double,double>(xmin,ymin);

  } 

}
