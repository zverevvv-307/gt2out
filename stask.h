#ifndef STASK_H
#define STASK_H

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/atomic.hpp>
#include "gtdtgrmqueue.h"

class SenderTask
{
    boost::asio::io_context& io_context;
    GtDtgrmQueue& queue_;

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;

    typedef GtDtgrmQueue::value_type DataType;
    DataType data_;
    void get_data();

    void handle_send(const boost::system::error_code& ec);
    void async_send();

    boost::asio::deadline_timer timer_;
    void handle_timeout(const boost::system::error_code &ec);
    void async_timeout();

public:
    boost::atomic_int errors_;
    boost::atomic_int count_;
    boost::atomic_int size_;

    SenderTask(boost::asio::io_context& ios, GtDtgrmQueue& q): io_context(ios)
      ,queue_(q)
      ,socket_(ios)
      ,remote_endpoint_()
      ,timer_(ios)
      ,errors_(0)
      ,count_(0)
      ,size_(0)
    { }

    bool open();

};

#endif // STASK_H
