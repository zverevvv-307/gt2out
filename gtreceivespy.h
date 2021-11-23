#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>



class GtReceiveSpy
{
  using Endpoint = boost::asio::ip::udp::endpoint;

  boost::asio::ip::udp::socket socket;
  boost::asio::ip::address listen_address;
  Endpoint listen_endpoint;

  int rejim;
  std::ofstream ofs;

  boost::asio::ip::udp::endpoint sender_endpoint;
  std::array<char, 2048> data;

  void async_receive_from() {
    socket.async_receive_from(
        boost::asio::buffer(data), sender_endpoint,
        boost::bind(&GtReceiveSpy::handle_receive_from, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

  void handle_receive_from(const boost::system::error_code& error,  size_t bytes_recvd);

public:
  GtReceiveSpy(
      int r,
      boost::asio::io_service& io_service,
      const std::string& i_address,
      int r_port);

  ~GtReceiveSpy();

};

