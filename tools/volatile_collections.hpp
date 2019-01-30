

//vvec push
template<class T>
void vvec<T>::push(T & t) {
	// check to resize the collection
	if (end_ == cap_) resize();

	// add element to end of collection
	data_[end_++] = t;

	// update size
	size_++;
	nheld_++;
}

// vvec []
template<class T>
T & vvec<T>::operator[](unsigned i) {
	unsigned p = pos_+i;
	while (!mask_[p] && p < size_)
		p++;
	return data_[p];
}

//const  vvec []
template<class T>
const T & vvec<T>::operator[](unsigned i) const {
	unsigned p = pos_ + i;
	while (!mask_[p] && p < size_)
		p++;
	return data_[p];
}

// mask
template<class T>
void vvec<T>::mask(unsigned i) {
	unsigned p = pos_ + i;
	while (!mask_[p] && p < size_)
		p++;

	mask_[p] = false;

	find_pos();
	update_nheld();
}

// mask
template<class T>
void vvec<T>::mask(bool * b) {
	for (unsigned i = 0; i != size_; i++) {
		mask_[i + pos_] = b[i];
	}

	find_pos();
	update_nheld();
}


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
	unsigned p = pos_ + i;
	while (!mask_[p] && p < size_)
		p++;
	return data_[p];
}


// vqueue mask(i)
template<class T>
void vqueue<T>::mask(unsigned i) {

	unsigned p = pos_ + i;
	while (!mask_[p] && p < size_)
		p++;

	mask_[p] = false;

	find_pos();
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
S & vmap<T,S>::find(const T & t) {
  for ( unsigned i=0; i != size_; i++ )
    if ( t == data_[i].first && mask_[i]) return data_[i].second;

  return data_[size_].second;
}

// rfind
template<class T,class S>
T & vmap<T,S>::rfind(const S & s) {
  for ( unsigned i=0; i != size_; i++ ) 
    if ( s == data_[i].second && mask_[i]) return data_[i].first;

  return data_[size_].first;
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

