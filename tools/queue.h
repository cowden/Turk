#ifndef QUEUE_H
#define QUEUE_H

#include <vector>

namespace Turk {

class queue {
public:

  queue():ntask_(0),size_(50),loc_(1),q_(50) {}
  queue(unsigned size):size_(size){ q_.resize(size_); }
  queue(const queue & q):ntask_(q.ntask_),size_(q.size_),loc_(q.loc_),q_(q.q_) {}

  ~queue() {}

  inline virtual int pop() 
  { 
    return q_[loc_++]; 
  }

  inline virtual void push(int task) 
  {
    ntask_++;
    if ( ntask_ == size_ ) {
      size_ *= 2;
      q_.resize( size_ );
    }
    q_[ntask_] = task;
  }


  inline virtual int ntask() { return ntask_ - loc_; }
  

protected:

  unsigned ntask_;
  unsigned size_;
  unsigned loc_;
  
  std::vector<int> q_;

};


class cycle_queue: public queue {
public:  
  cycle_queue() {}
  cycle_queue(unsigned size) { }
  cycle_queue(const cycle_queue & q ) { }

  ~cycle_queue() { }

  inline virtual void init_cycle() { ncycle_ = ntask_ + 1; }

  unsigned virtual int cycle_pop()
  {
    if ( loc_ == ncycle_ ) {
      ncycle_ = ntask_ + 1;
      return -1;
    } else {
      return pop();
    }
  }
  

private:

  unsigned ncycle_;
  
};

} // end Turk namespace

#endif
