#ifndef KDTREE_H
#define KDTREE_H

/////////////////////////////
// C S Cowden  14 April 2018
// KD-tree implementation.
// I've implemented (in C++) the example code provided in 
// Numerical Recipes.
/////////////////////////////

#include <vector>
#include <cassert>
#include <cmath>


#define SQR(a) a*a

namespace Turk {

class kdpoint {

public:

  kdpoint() {}

  kdpoint(const kdpoint &pt): dim_(pt.dim_)
    {
      x_.resize(dim_);
      for ( unsigned i=0; i != dim_; i++ ) 
        x_[i] = pt.x_[i];
    }

  kdpoint(const unsigned dim):dim_(dim) {
    x_.resize(dim_);
  }

  kdpoint(const unsigned dim,const double * x):dim_(dim) {
    x_.resize(dim_);
    for ( unsigned i=0; i != dim_; i++ )
      x_[i] = x[i];
  }

  kdpoint & operator=(const kdpoint &pt) 
    {
      for ( unsigned i=0; i != dim_; i++ ) x_[i] = pt.x_[i];
      return *this;
    }

  bool operator==(const kdpoint &pt) {
    for ( unsigned i=0; i != dim_; i++ ) if ( x_[i] != pt.x_[i] ) return false;
    return true;
  }

  double & operator[](const unsigned i) { return x_[i]; }

  double dist(const kdpoint &p) const {
    double dd = 0.;
    for ( unsigned j=0; j != dim_; j++ ) dd += SQR(x_[j]-p.x_[j]);
    return sqrt(dd);
  }


  const unsigned dim() const { return dim_; }

  const double * data() const { return &x_[0]; }


private:

  unsigned dim_;
  std::vector<double> x_;

};



class kdbox {

public:


  kdbox() { }

  kdbox(const kdpoint & lo, const kdpoint  & hi, int mom, int dau1, int dau2, int ptlo, int pthi):
    low_(lo),high_(hi),mom_(mom),dau1_(dau1),dau2_(dau2),ptlo_(ptlo),pthi_(pthi) { 

    assert(lo.dim() == hi.dim());

    dim_ = lo.dim();

  }



  double dist(const kdpoint &p) {

    assert(dim_ == p.dim());

    const double * pdata = p.data();

    double dd = 0.;
    for ( unsigned i=0; i != dim_; i++ ) {
      if ( pdata[i] < low_.data()[i] ) dd += SQR(pdata[i]-low_.data()[i]);
      if ( pdata[i] > high_.data()[i] ) dd += SQR(pdata[i]-high_.data()[i]);
    }
    return sqrt(dd);
  }

  void set_dau1(const int dau ) { dau1_ = dau; }
  void set_dau2(const int dau) { dau2_ = dau; }

  
  int ptlo() const { return ptlo_; }
  int pthi() const { return pthi_; }
  int mom() const { return mom_; }
  int dau1() const { return dau1_; }
  int dau2() const { return dau2_; }


  kdpoint & low() { return low_; }
  kdpoint & high() { return high_; }  

private:

  unsigned dim_;
  kdpoint low_, high_;
  int mom_,dau1_,dau2_, ptlo_, pthi_; 

};

//


class kdtree {
public:

  /**
  * construct the kdtree
  */
  kdtree(const double * data, const unsigned n_features, const unsigned n_obs);

  /**
  * Return the  nearest neighbors to a point.
  */


  /**
  * Return the points with in a given radius of the supplied location.
  */

private:


  // --- member functions ---
  int selecti(unsigned k, unsigned * indices, unsigned n, double * pts);


  // points
  unsigned dim_;
  std::vector<kdpoint> points_;

  // coordinates
  double * coords_;

  // index and reverse index
  std::vector<unsigned> index_;
  std::vector<unsigned> rindex_;

  // boxes
  std::vector<kdbox> boxes_;

  // size of universal box
  static const double BIG;

};


} // end Turk namespace

#endif
