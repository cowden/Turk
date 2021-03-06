

#include "kdtree.h"

#include <algorithm>


using namespace Turk;

const double kdtree::BIG = 1.e99;

kdtree::kdtree(const double * data, const unsigned n_features, const unsigned n_obs) {

  dim_ = n_features;
  index_ = std::vector<unsigned>(n_obs);
  rindex_ = std::vector<unsigned>(n_obs);

  int ntmp, m, nowtask, jbox, np, tmom, tdim, ptlo, pthi;
  unsigned *hp;
  double *cp;
  int taskmom[50], taskdim[50];
  for ( unsigned k=0; k<n_obs; k++ ) index_[k] = k;

  // calculate number of boxes and allocate memory
  m = 1;
  for ( ntmp = n_obs; ntmp; ntmp >>= 1) {
    m <<= 1;
  }
  int nboxes = 2*n_obs - (m >> 1);
  if ( m < nboxes ) nboxes = m;
  nboxes--;
  nboxes = 1e4;
  boxes_ = std::vector<kdbox>(nboxes);

  // copy the data into a contiguous array
  std::vector<double> coords(n_features*n_obs);
  for ( unsigned kk=0; kk != n_features; kk++ ) {
    for ( unsigned k=0; k != n_obs; k++ ) 
      coords[kk*n_obs+k] = data[k*n_features+kk];
  }

  // copy data as kdpoint vector
  points_.resize(n_obs);
  for ( unsigned kk=0; kk != n_obs; kk++ )
    points_[kk] = kdpoint(n_features,&data[kk*n_features]);


  // initialize the root box and put it on the task list for subdivision
  kdpoint lo(dim_), high(dim_);
  for ( unsigned i=0; i != dim_; i++ ) {
    lo[i] = -BIG;
    high[i] = BIG;
  }


  boxes_[0] = kdbox(lo, high, 0, 0, 0, 0, n_obs-1);
  jbox = 0;
  taskmom[1] = 0;
  taskdim[1] = 0;
  nowtask = 1;
  unsigned kk=0;
  while ( nowtask ) {
    tmom = taskmom[nowtask];
    tdim = taskdim[nowtask--];
    ptlo = boxes_[tmom].ptlo();
    pthi = boxes_[tmom].pthi();
    hp = &index_[ptlo];
    cp = &coords[tdim*n_obs];
    np = pthi - ptlo + 1;
    kk = (np-1)/2;
    if ( np ) {
      (void) selecti(kk,hp,np,cp);
    high = boxes_[tmom].high();
    lo = boxes_[tmom].low();
    high[tdim] = lo[tdim] = coords[tdim*n_obs + hp[kk]];
    boxes_[++jbox] = kdbox(boxes_[tmom].low(),high,tmom,0,0,ptlo,ptlo+kk);
    boxes_[++jbox] = kdbox(lo,boxes_[tmom].high(),tmom,0,0,ptlo+kk+1,pthi);
    boxes_[tmom].set_dau1(jbox-1);
    boxes_[tmom].set_dau2(jbox);
    if ( kk > 1 ) {
      taskmom[++nowtask] = jbox-1;
      taskdim[nowtask] = (tdim+1) % dim_;
    }
    if ( np - kk > 0 ) {
      taskmom[++nowtask] = jbox;
      taskdim[nowtask] = (tdim+1) % dim_;
    }
    }
  }

  for ( unsigned j=0; j < n_obs; j++ ) rindex_[index_[j]] = j;

}



int kdtree::selecti(unsigned k, unsigned * indices, unsigned n, double * pts) {

  int i, ia, ir, j, l, mid;
  double a;

  l=0;
  ir=n-1;
  for (;;) {
    if ( ir <= l+1 ) {
      if ( ir == l+1 && pts[indices[ir]] < pts[indices[l]] )
        std::swap(indices[l],indices[ir]);
      return indices[k];
    } else {
      mid = (l+ir) >> 1;
      std::swap(indices[mid],indices[l+1]);
      if ( pts[indices[l]] > pts[indices[ir]] ) std::swap(indices[l],indices[ir]);
      if ( pts[indices[l+1]] > pts[indices[ir]] ) std::swap(indices[l+1],indices[ir]);
      if ( pts[indices[l]] > pts[indices[l+1]] ) std::swap(indices[l],indices[l+1]);
      i=l+1;
      j = ir;
      ia = indices[l+1];
      a = pts[ia];
      for ( ;; ) {
        do i++; while (pts[indices[i]] < a);
        do j--; while (pts[indices[j]] > a);
        if ( j < i) break;
        std::swap(indices[i],indices[j]);
      }
      indices[l+1] = indices[j];
      indices[j] = ia;
      if ( j >= k ) ir=j-1;
      if ( j <= k ) l=i;
    }
  }

}




int kdtree::ballsearch(const kdpoint & pt, double r, unsigned * list, unsigned limit ) {

  // find the smallest box  that contains the ball
  unsigned nb = 0;
  unsigned jdim = 0;

  while(boxes_[nb].dau1()) {

    unsigned nbold = nb;
    int d1 = boxes_[nb].dau1();
    int d2 = boxes_[nb].dau2();

    const double x = pt.data()[jdim];
    if ( x + r <= boxes_[d1].high().data()[jdim] ) nb = d1;
    else if ( x - r >= boxes_[d2].low().data()[jdim] ) nb = d2;
    
    jdim = ++jdim % dim_;

    if ( nb == nbold ) break;

  }
  
  // now traverse the tree below the starting box only as needed
  unsigned ntask = 1;
  unsigned task[50];
  int nret = 0;

  task[1] = nb;
  while ( ntask ) {
    unsigned k = task[ntask--];
    if ( boxes_[k].dist(pt) > r ) continue;
    if ( boxes_[k].dau1() ) {
      task[++ntask] = boxes_[k].dau1();
      task[++ntask] = boxes_[k].dau2();
    } else {
      for ( unsigned i=boxes_[k].ptlo(); i <= boxes_[k].pthi(); i++ ) {
        if ( points_[index_[i]].dist(pt) <= r && nret < limit ) 
          list[nret++] = index_[i];
        if ( nret == limit ) return limit;
      }
    }
  }
   
  return nret; 

}
