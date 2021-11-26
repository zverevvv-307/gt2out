#pragma once

#include <boost/asio.hpp>
#include <atomic>
#include <memory>
#include "filter.h"


class GtAsioIpxSocket;
class GtEthernet;

class ReceiveTask
{
  boost::asio::io_context& io_context;
  FilterPtr filter_;

  std::unique_ptr<GtAsioIpxSocket>   ipx_;
  std::unique_ptr<GtEthernet>        mcast_;

  void processI(const TDatagram &dtg);
  void processI2(const TDatagram2 &dtg2);

public:
  std::atomic_int count_;

  ReceiveTask(boost::asio::io_context& ios, FilterPtr f);
  ~ReceiveTask();
  bool open();
};

