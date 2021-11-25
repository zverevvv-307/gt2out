#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <mutex>
#include <boost/asio.hpp>

#include "dmpacket2.h"
#include "opt.h"
#include "../gtlan/gteth.h"



class GtClient
{
  boost::asio::ip::udp::socket isocket_;
  boost::asio::ip::address listen_address_;
  boost::asio::ip::udp::endpoint listen_endpoint_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
  std::array<char, 2048> data;

  void async_receive_from();
  void handle_receive_from(const boost::system::error_code& error,  size_t bytes_recvd);

  GtEthernet   mcast_;

  using StorageKey = std::pair<int, std::string>;
  using StorageMap = std::map< StorageKey, TDatagramPacket2 >;
  StorageMap storage_;
  std::mutex mtx_;

  TDatagramPacket2& get(const StorageKey& key) {
    std::lock_guard<std::mutex> guard(mtx_);
    return storage_[key];
  }

public:
  GtClient( boost::asio::io_service& io_service );

};

