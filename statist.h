#pragma once

#include <array>
#include <string>
#include <boost/asio.hpp>


class Statistics
{
    void handle_timeout(const boost::system::error_code& error);
    boost::asio::deadline_timer timer_;

    using statists_type = std::array<std::atomic_int*, 3>;
    int dt_milliseconds;

public:
    statists_type data_;

    Statistics(boost::asio::io_context& io_context , int dt = 10*1000);

    void print_and_clear();
};

