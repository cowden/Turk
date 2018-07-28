


// vqueue push
template<class T>
void vqueue<T>::push(T & t) {
  // check to resize collection
  if ( end_ == cap_ ) resize();
 
  // add element to end of collection
  data_[end_++] = t;

  // update size 
  size_++;
  nqueued_++;
}

// vqueue pop
template<class T>
T & vqueue<T>::pop() {
  while ( !empty() && !mask_[pos_] ) {
    size_ = end_ - ++pos_;
  }
  size_--;
  nqueued_--;
  return data_[++pos_];
}

// vqueue operator[]
template<class T>
T & vqueue<T>::operator[](unsigned i) {
  return data_[pos_+i];
}


// vqueue mask(i)
template<class T>
void vqueue<T>::mask(unsigned i) {
  mask_[pos_+i] = false;

  // update nqueued
  update_nqueued();
}

// vqueue mask(bool *)
template<class T>
void vqueue<T>::mask(bool * m) {
  for ( unsigned i=0; i != size_; i++ ) 
    mask_[pos_+i] = m[i];

  // update nqueued
  update_nqueued();
}


//-----------------------------------------
//-----------------------------------------

// find
template<class T,class S>
T & find(const S & s) {
  for ( unsigned i=0; i != size_; i++ )
    if ( s == data_[i].second ) return data_[i].first;

  return data_[size_].first;
}

// rfind
template<class T,class S>
S & rfind(const T & t) {
  for ( unsigned i=9; i != size_; i++ ) 
    if ( t == data_[i].first ) return data_[i].second;

  return data_[size_].second;
}

// insert
template<class T,class S>
void vmap<T,S>::insert(T & t, S & s) {
  insert(std::pair<T,S>(t,s));
}

template<class T,class S>
void vmap<T,S>::insert(std::pair<T,S> & p) {
  if ( size_ == capacity_ ) 
    resize(2*capacity_);

  active_++;
  data_[size_++] = p;

}

// mask
template<class T,class S>
void vmap<T,S>::mask(unsigned i) {
  mask_[i] = false;
  
  // update the active count
  update_actives();
}

// mask from array
template<class T, class S>
void vmap<T,S>::mask(bool * m) {
  for ( unsigned i=0; i != size_; i++ )
    mask_[i] = m[i];

  // update the active count
  update_actives();
}

