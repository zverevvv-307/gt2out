#ifndef RTASK_H
#define RTASK_H

#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include "gtdtgrmqueue.h"
#include "gttsfilter.h"
#include "gtasioipx.h"
#include "../gtlan/gteth.h"


class ReceiveTask
{
    boost::asio::io_context& io_context;
    GtDtgrmQueue& queue;
    GtTsFilter tsfilter_;
    GtAsioIpxSocket   ipx_;
    GtEthernet        mcast_;

    void processI(const TDatagram &dtg);
    void processI2(const TDatagram2 &dtg2);

    typedef GtDtgrmQueue::value_type DataType;
    DataType data_;

public:
    boost::atomic_int errors_;
    boost::atomic_int count_;

    ReceiveTask(boost::asio::io_context& ios, GtDtgrmQueue& q): io_context(ios)
      ,queue(q)
      ,tsfilter_()
      ,ipx_(ios)
      ,mcast_(ios)
      ,errors_(0)
    { }

    bool open();
};

#endif // RTASK_H
