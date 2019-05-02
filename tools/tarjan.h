#ifndef TARJAN_H
#define TARJAN_H

namespace Turk {

class tarjan {
public:

  //
  tarjan() { }

  //
  template<class T>
  inline void fit(std::vector<T> & graph_data) {

    N_ = sqrt(graph_data.size());

    // resize elements
    vi_.resize(N_);
    depth_.resize(N_);
    low_.resize(N_);
    parent_.resize(N_,-1);

    for ( unsigned i=0; i != N_; i++ ) {
      vi_[i] = 0;
      depth_[i] = 0;
      low_[i] = 0;
      parent_[i] = -1;
    }
   
    // search until all points have been visited.
    unsigned i = 0;
    while ( sum(vi_) < N_ ) { 

      if ( !vi_[i] ) {
        // begin recursive search
        search(graph_data,i,0);
      }
      i++;

    }
  }

  //
  const std::vector<unsigned> & articulation_points() const { return ap_; }

private:

  //
  template<class T>
  inline void search(const std::vector<T> & gd,unsigned vtx, unsigned d) {

    depth_[vtx] = d;
    low_[vtx] = d;
    vi_[vtx]= 1;

    unsigned childcount = 0;

    // cycle over children
    for ( unsigned i=0; i != N_; i++ ) {
      if ( i == vtx || gd[vtx*N_+i] == 0 ) continue;
      if ( vi_[i] == 0 ) {
        parent_[i] = vtx;
        search(gd,i,d+1);
        childcount++;

        if ( low_[i] >= depth_[vtx] && parent_[vtx] != -1 )
          ap_.push_back(vtx);

        low_[vtx] = low_[i] < low_[vtx] ? low_[i] : low_[vtx];

      } else if ( i != parent_[vtx] ) {
        low_[vtx] = low_[vtx] < depth_[i] ? low_[vtx] : depth_[i];
      }
    }

    if ( parent_[vtx] == -1 && childcount > 1 )
      ap_.push_back(vtx);
     
  
  }

  //
  inline unsigned sum(const std::vector<unsigned> & v) const { 
    unsigned s = 0;
    const unsigned n = v.size();
    for ( unsigned i=0; i != n; i++ ) 
      s += v[i];

    return s;
  }

  // graph size
  unsigned N_;

  // visited
  std::vector<unsigned> vi_;

  // articulation points
  std::vector<unsigned> ap_;

  // depth
  std::vector<unsigned> depth_;

  // low
  std::vector<unsigned> low_;

  // parent
  std::vector<int> parent_;



};

}  // end Turk namespace

#endif
