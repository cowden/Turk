#pragma once

/***************************
* C S Cowden         23 July 2018
* This header defines a set of 
* collections, stacks, queues, and maps
* for elements which are volatile
* (i.e. StarCraft units which tend to die)
****************************/


#include <vector>

namespace Turk {

/**
* class to hold a collection of volatile items
*/
template<class T>
class vvec {
public:
	/**
	* default constructor
	*/
	inline vvec() :size_(0U), nheld_(0U), cap_(50U), pos_(0U), end_(0U)
		,mask_(50U,true),data_(50U)
	{}

	/**
	* constructor initialize the size
	*/
	inline vvec(unsigned n) :size_(0U), nheld_(0U), cap_(n), pos_(0U), end_(0U)
		,mask_(n,true),data_(n)
	{}

	/**
	* push an item onto the back of the collection
	*/
	inline void push(T &t);

	/**
	* get the size of the container
	*/
	inline unsigned size() const { return size_; }

	/**
	* get the number of unmasked items
	*/
	inline unsigned nheld() const { return nheld_; }

	/**
	* return true if empty
	*/
	inline bool empty() const { return size_ == 0U; }

	/**
	* access elements []
	*/
	inline T & operator[](unsigned i);

	/**
	* const accessor
	*/
	const inline const T & operator[](unsigned i) const;

	/**
	* return if an element is masked
	*/
	inline bool getMask(unsigned i) const { return mask_[pos_+i]; }

	/**
	* mask an element
	*/
	inline void mask(unsigned i);

	/**
	* set mask for the entire collection
	*/
	inline void mask(bool * m);

private:

	// resize the collections
	inline void resize() {
		cap_ *= 2;
		mask_.resize(cap_);
		data_.resize(cap_);
	}

	// update the number of held items
	inline void update_nheld() {
		nheld_ = size_ - pos_;
		for (unsigned i = pos_; i != end_; i++)
			if (!mask_[i]) nheld_--;
	}

	// find the first non-masked postion
	inline void find_pos() {
		pos_ = 0U;
		while (!mask_[pos_] && pos_ < size_)
			pos_++;
	}

	// size of the collection (total)
	unsigned size_;

	// number of unmasked items
	unsigned nheld_;

	// capacity of underlying container
	unsigned cap_;

	// first unmasked position
	unsigned pos_;

	// last element
	unsigned end_;

	// maks
	std::vector<bool> mask_;

	// collection
	std::vector<T> data_;


};


/**
* Class to queue volatile items 
* (that is items that may die in the queue).
*/
template<class T>
class vqueue {

public:

  /**
  * default constructor
  */
  inline vqueue():size_(0U),nqueued_(0U),cap_(50U),pos_(0U),end_(0U) 
  ,mask_(50U,true),data_(50U)
  { }   
  

  /**
  * constructor initialize the 
  * container size.
  */
  inline vqueue(unsigned n):size_(0U),nqueued_(0U),cap_(n),pos_(0U),end_(0u)
  ,mask_(n,true),data_(n)
  { }
  
  /**
  * return the size of the queue, ignoring masked off elements
  */
  inline unsigned size() const { return size_; }

  /**
  * return the number of queued items (size - masked items).
  */
  inline unsigned nqueued() const { return nqueued_; }

  /**
  * push an element on the queue
  */
  inline void push(T &);

  /**
  * pop an element off the from of the queue
  */
  inline T & pop();

  /**
  * return true of the queue is empty
  */
  inline bool empty() const { return size_ == 0U; }


  /**
  * allow external iteration to mark dead items
  * Note: the argument i refers to the item in the queue
  * and not the element in the underlying collection.
  */
  inline T & operator[](unsigned i);


  /**
  * mask off element.
  */
  inline void mask(unsigned i);

 
  /**
  * mask of elements.
  */
  inline void mask(bool *); 


private:

  // resize the collection if necessary
  inline void resize() {
    cap_ *= 2U;
    mask_.resize(cap_);
    data_.resize(cap_);
  }

  // update nqueued
  inline void update_nqueued() {
    nqueued_ = size_;
    for ( unsigned i=pos_; i != end_; i++ )
      if ( !mask_[i] ) nqueued_--;
  }

  // find position of first unmasked item
  inline void find_pos() {
	  while (!mask_[pos_] && pos_ < size_)
		  pos_++;
  }

  // size
  unsigned size_;

  // nqueued
  unsigned nqueued_;

  // capacity
  unsigned cap_;

  // position
  unsigned pos_;

  // end
  unsigned end_;

  // mask
  std::vector<bool> mask_;

  // collection
  std::vector<T> data_;



};




template<class T>
class vstack {
};



/**
* Map T to S for volatile items (that is items that can die).
*/
template<class T,class S>
class vmap {

public:

  /**
  * default constructor
  */
  inline vmap():size_(0u),active_(0U),capacity_(50U) 
  { 
    // fill initial mask
    mask_ = std::vector<bool>(capacity_,true);

    // initialize the data collection
    data_ = std::vector<std::pair<T,S> >(capacity_);
  }


  /**
  * constructor initializing the container size
  */
  inline vmap(unsigned n):size_(0U),active_(0U),capacity_(n) 
  { 
    // fill initial mask
    mask_ = std::vector<bool>(capacity_,true);

    // initialize the data collection
    data_ = std::vector<std::pair<T,S> >(capacity_);
  }


  /**
  * return the size of the map
  */
  inline unsigned size() const { return size_; }

  /** 
  * return the list of active elements
  */
  inline unsigned active() const { return active_; }


  /**
  * return an element of the map
  */
  inline std::pair<T,S> & operator[](unsigned i) { return data_[i]; }

  /**
  * return the mask of the map element
  */
  inline bool ismask(unsigned i) { return mask_[i]; }

  /**
  * find S from T
  */
  inline S & find(const T &);

  /**
  * find T from S (reverse find)
  */
  inline T & rfind(const S & s);

  /**
  * insert a new element
  */
  inline void insert(T & t, S & s);

  /**
  * insert from a pair
  */
  inline void insert(std::pair<T,S> & p);

  /**
  * resize the collections
  */
  inline void resize(unsigned n) {
    capacity_ = n;
    mask_.resize(n);
    data_.resize(n);
  }

  /**
  * mask a particular element
  */
  inline void mask(unsigned i);

  /**
  * mask a series of elements
  */
  inline void mask( bool * m );

  /**
  * set method.  This is assigning the value returned by find.
  */
  inline void set(const T & t,const S & s) {
	  for (unsigned i = 0; i != size_; i++)
		  if (t == data_[i].first && mask_[i]) data_[i].second = s;
  }


  /**
  * set method.  This is assigning the value returned by rfind.
  */
  inline void set(const S & s, const T & t) {
	  for (unsigned i = 0; i != size_; i++)
		  if (s == data_[i].second && mask_[i]) data_[i].first = t;
  }

  
private:

  // update the active count
  inline void update_actives() {
    active_ = size_;
    for ( unsigned i=0; i != size_; i++ )
      if ( !mask_[i] ) active_--;
  }

  // size of map
  unsigned size_;
 
  // number of active (unmasked elements)
  unsigned active_;

  // capacity
  unsigned capacity_;

  // mask
  std::vector<bool> mask_;
 
  // collection 
  std::vector<std::pair<T,S> > data_;

};


// include the inline definitions
#include "volatile_collections.hpp"


} // end Turk namespace

