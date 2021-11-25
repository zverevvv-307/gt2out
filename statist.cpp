#include "statist.h"
#include "helpers.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <boost/bind/bind.hpp>


Statistics::Statistics( boost::asio::io_context& io_context, int dt )
    : timer_( io_context ),
      dt_milliseconds(dt),
      data_()
{
  async_loop();
}

void Statistics::async_loop()
{
  timer_.expires_from_now(std::chrono::milliseconds(dt_milliseconds));
  timer_.async_wait( [this](const boost::system::error_code& ec){
    if(ec) return;
    this->print_and_clear();
    async_loop();
  });
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
          ,tmp[1]
          ,tmp[2]*1000/dt_milliseconds
          );
  std::cout << Vx::now_time().c_str() << " " << &txt[0] << std::endl;
}

