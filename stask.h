#pragma once

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <atomic>
#include "filter.h"

class SenderTask
{
  boost::asio::io_context& io_context;

  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint remote_endpoint_;

  boost::asio::steady_timer timer_;
  void send_all();
  void async_loop();

  FilterPtr filter_;

public:
  std::atomic_int count_=0;
  std::atomic_int size_=0;

  SenderTask(boost::asio::io_context& ios, FilterPtr f): io_context(ios)
                                                          ,socket_(ios)
                                                          ,remote_endpoint_()
                                                          ,timer_(ios)
                                                          ,filter_(f)
  { }

  bool open();

};

