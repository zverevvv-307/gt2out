#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include "opt.h"
#include "../gtlan/gteth.h"



class GtClient
{
  using Endpoint = boost::asio::ip::udp::endpoint;

  boost::asio::ip::udp::socket socket;
  boost::asio::ip::address listen_address;
  Endpoint listen_endpoint;

  boost::asio::ip::udp::endpoint sender_endpoint;
  std::array<char, 2048> data;

  void async_receive_from();
  void handle_receive_from(const boost::system::error_code& error,  size_t bytes_recvd);

  GtEthernet   mcast_;

public:
  GtClient( boost::asio::io_service& io_service );

};

