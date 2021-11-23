//----------------------------------------------------------------------------
//  Copyright 2013. All Rights Reserved.
//  Author:   Vladislav Zverev
//----------------------------------------------------------------------------
#ifndef STATIST_H
#define STATIST_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/atomic.hpp>
#include "gtdtgrmqueue.h"


class Statistics
{
    void handle_timeout(const boost::system::error_code& error);
    boost::asio::deadline_timer timer_;

    typedef std::vector<boost::atomic_int*> statists_type;

    int dt_milliseconds;

public:
    statists_type data_;

    Statistics(boost::asio::io_context& io_context , int dt = 10*1000);

    void print_and_clear();
};

#endif // STATIST_H
