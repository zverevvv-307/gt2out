#include "statist.h"
#include "helpers.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <boost/bind/bind.hpp>


Statistics::Statistics( boost::asio::io_context& io_context, int dt )
    : timer_( io_context )
      , data_()
{
  dt_milliseconds = dt;
  timer_.expires_from_now( boost::posix_time::milliseconds(dt_milliseconds) );
  timer_.async_wait( boost::bind(&Statistics::handle_timeout, this, boost::asio::placeholders::error) );
}

void Statistics::handle_timeout(const boost::system::error_code& error)
{
  if (!error) {
    print_and_clear();
    timer_.expires_from_now( boost::posix_time::milliseconds(dt_milliseconds) );
    timer_.async_wait( boost::bind(&Statistics::handle_timeout, this, boost::asio::placeholders::error) );
  }
}

void Statistics::print_and_clear()
{
  std::vector<int> tmp(data_.size());
  for(size_t i=0; i<data_.size(); ++i){
    tmp[i] = *data_[i];
    *data_[i]=0;
  }

  //печатаем сохраненные
  std::vector<char> txt(1024, 0);
  sprintf(&txt[0],
          "in: %3d "
          "out: %3d "
          "-> %3d B/s"
          ,tmp[0]
          ,tmp[1],tmp[2]*1000/dt_milliseconds
          );
  std::cout << Vx::now_time().c_str() << " " << &txt[0] << std::endl;
}

