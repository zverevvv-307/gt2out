#include "stask.h"
#include "opt.h"

#include <boost/bind/bind.hpp>

using namespace std;
namespace ba=boost::asio;


#define print_ec() \
    if (ec) { \
    printf( "%s: %d %s", __PRETTY_FUNCTION__, ec.value(), ec.message().c_str() ); \
    ec.clear(); \
    return false; }

bool SenderTask::open()
{
    ba::ip::address remote_address = ba::ip::address::from_string( Options::instance()->remote_address );
    unsigned short remote_port = Options::instance()->remote_port;

    boost::system::error_code ec;
    remote_endpoint_ = ba::ip::udp::endpoint(remote_address, remote_port);
    socket_.open(remote_endpoint_.protocol(), ec);
    print_ec();
    if (remote_address.is_v4() && remote_address.to_v4().is_multicast()) {
        socket_.set_option(ba::ip::multicast::hops(4), ec);
        print_ec();
        ba::ip::multicast::outbound_interface ointerface1(ba::ip::address_v4::from_string( Options::instance()->local_address_kit) );
        socket_.set_option(ointerface1, ec);
        print_ec();
    } else {
        socket_.set_option(ba::ip::unicast::hops(4), ec);
        print_ec();
    }

    printf("opened sender/out point is : %s:%d", remote_endpoint_.address().to_string().c_str(), remote_endpoint_.port() );

    async_timeout();
    return true;
}

void SenderTask::get_data()
{
    if( !queue_.pop( data_ ) )
        async_timeout();
    else async_send();
}

void SenderTask::handle_send(const boost::system::error_code& ec)
{
    if (ec) {
        ++errors_;
        printf( "%s: %d %s", __PRETTY_FUNCTION__, ec.value(), ec.message().c_str() ); \
    }
    else {
        ++count_;
    }
    get_data();
}

void SenderTask::async_send()
{
    int sz = TDatagram2_HEADER_LN + data_.Size;
    size_ += sz;
    socket_.async_send_to( ba::buffer( &data_,  sz ), remote_endpoint_,
                           boost::bind(&SenderTask::handle_send, this, ba::placeholders::error)
                           );
}

void SenderTask::handle_timeout(const boost::system::error_code& ec)
{
    if(!ec)
        get_data();
}

void SenderTask::async_timeout()
{
    timer_.expires_from_now(boost::posix_time::milliseconds(200));
    timer_.async_wait( boost::bind( &SenderTask::handle_timeout, this, ba::placeholders::error ) );
}

