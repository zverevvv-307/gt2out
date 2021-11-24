#pragma once

#include <boost/asio.hpp>
#include <atomic>
#include "filter.h"
#include "gtasioipx.h"
#include "../gtlan/gteth.h"


class ReceiveTask
{
    boost::asio::io_context& io_context;
    FilterPtr filter_;
    GtAsioIpxSocket   ipx_;
    GtEthernet        mcast_;

    void processI(const TDatagram &dtg);
    void processI2(const TDatagram2 &dtg2);

public:
    std::atomic_int count_;

    ReceiveTask(boost::asio::io_context& ios, FilterPtr f): io_context(ios)
      ,filter_(f)
      ,ipx_(ios)
      ,mcast_(ios)
    { }

    bool open();
};

