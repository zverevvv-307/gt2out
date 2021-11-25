#pragma once

#include "rtask.h"
#include "stask.h"
#include "statist.h"
#include "filter.h"

#include "opt.h"
#include "asio_pch.h"


class GtServer
{
  FilterPtr filter_;
  std::unique_ptr<SenderTask> stask;
  std::unique_ptr<ReceiveTask> rtask;
  std::unique_ptr<Statistics> statist;

public:
  GtServer( boost::asio::io_service& io_service )
      : filter_( std::make_shared<Filter>() ) {
    if( !filter_->open() )
      throw std::runtime_error("Filter failed.");
    stask = std::make_unique<SenderTask>(io_service, filter_);
    if( !stask->open() )
      throw std::runtime_error("SenderTask failed.");
    rtask = std::make_unique<ReceiveTask>(io_service, filter_);
    if( !rtask->open() )
      throw std::runtime_error("ReceiveTask filed");

    statist= std::make_unique<Statistics>(io_service);
    statist->data_[0] = &rtask->count_;
    statist->data_[1] = &stask->count_;
    statist->data_[2] = &stask->size_;

    std::cout <<std::endl<<"Server started."<< std::endl<< std::endl;
  }
};

