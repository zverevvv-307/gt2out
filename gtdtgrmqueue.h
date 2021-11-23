#ifndef GTDTGRMQUEUE_H
#define GTDTGRMQUEUE_H


#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>

template<typename T>
class lockfree_queue_
{
    boost::lockfree::queue<T> queue_;

public:
    boost::atomic_int count_;
    typedef T  value_type;

    lockfree_queue_(int sz=1024): queue_(sz), count_(0) {}

    bool push(const T& value){
         int i = 7;
         while ( i && !queue_.push(value))
                     --i;
         if(i)
             ++count_;
         return i;
    }

    bool pop(T& value){
         bool r = queue_.pop(value);
         if(r)
            --count_;
         return r;
    }

    int count()
    {
        return count_;
    }
};


#include "dmpacket2.h"
typedef lockfree_queue_<TDatagram2>  GtDtgrmQueue;




#endif // GTDTGRMQUEUE_H
