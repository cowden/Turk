#ifndef POLYARTIE_H
#define POLYARTIE_H

#include <vector>

#include "linear_model.h"

namespace Turk {


/**
* The polyartie class exists
* to simply to fit a polynomial to a window around a pixel
* and return if the minima occurs with in the
* center pixel.
* That is its sole mission in life, and I intend to 
* help it acheive this.
*/
class polyartie:public linear_model {

public:

  /**
  * Construct the fitter from x and y values of the window as well as
  * the distance values in the window.
  * Then perform the fit.
  */
  polyartie(const std::vector<double> & 
    ,const std::vector<double> &
    ,const std::vector<double> & );

  /**
  * Calculate the minium of the fit, and fill the value in members.
  */
  void calcMinima();


  /**
  * Return true/false if the pixel is a minima.
  */
  bool isMinima(); 

  
  /**
  * Return the minimum.
  */
  inline const std::pair<double,double> &  getMinima() const { return m_min; }
  

private:

  std::pair<double,double> m_min; 

};

} // end Turk namespace

#endif

