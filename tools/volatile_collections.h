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
  inline void push(T);

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
  * find T from S
  */
  inline T & find(const & S);

  /**
  * find S from T (reverse find)
  */
  inline S & rfind(const T & t);

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

