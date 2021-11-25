#pragma once

#include <array>
#include <string>
#include <boost/asio.hpp>


class Statistics
{
  boost::asio::steady_timer timer_;
  int dt_milliseconds;

  using statists_type = std::array<std::atomic_int*, 3>;
  void async_loop();

public:
  statists_type data_;

  Statistics(boost::asio::io_context& io_context , int dt = 10*1000);

  void print_and_clear();
};

