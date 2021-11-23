//----------------------------------------------------------------------------
//  Copyright 2013. All Rights Reserved.
//  Author:   Vladislav Zverev
//----------------------------------------------------------------------------
#include "statist.h"
#include "helpers.h"

//#include "logger.h"
//static logger::Logger& slogger = logger::Logger::getLogger("statist");

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
    const int statist_size=6;

    std::vector<int> tmp(statist_size);
    //сохраняем и чистим все счетчики кроме длины очереди
    for(int i=0; i<statist_size; ++i){
        boost::atomic_int *ai = data_[i];
        if(!ai) break;
        //не читим каунтер очереди
        if(i) tmp[i]= (*ai).exchange(0);
        else  tmp[i]= (*ai).load();
    }
    //печатаем сохраненные
    std::vector<char> txt(1024, 0);
    sprintf(&txt[0],
            "q: %3d "
            "in: %3d[%3d] "
            "out: %3d[%3d] "
            "-> %3d B/s"
            ,tmp[0]
            ,tmp[1],tmp[2]
            ,tmp[3],tmp[4]
            ,tmp[5]*1000/dt_milliseconds
            );
    std::cout << Vx::now_time().c_str() << " " << &txt[0] << std::endl;
}

