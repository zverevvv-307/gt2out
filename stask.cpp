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
    ba::ip::multicast::outbound_interface ointerface1(ba::ip::address_v4::from_string( Options::instance()->local_address_wan) );
    socket_.set_option(ointerface1, ec);
    print_ec();
  } else {
    socket_.set_option(ba::ip::unicast::hops(4), ec);
    print_ec();
  }

  printf("opened sender/out point is : %s:%d", remote_endpoint_.address().to_string().c_str(), remote_endpoint_.port() );

  async_loop();
  return true;
}


void SenderTask::send_all()
{
  filter_->for_each([this](Filter::FilterData& rec){
    TDatagram2& data = rec.store.Dtgrm;
    int sz = TDatagram2_HEADER_LN + data.Size;
    this->socket_.send_to( ba::buffer( &data,  sz ), remote_endpoint_ );
    this->count_++;
    this->size_ += sz;
  });
}

void SenderTask::async_loop()
{
  timer_.expires_from_now(std::chrono::seconds(Options::instance()->timeout_sec));
  timer_.async_wait( [this](const boost::system::error_code& ec){
    if(ec) return;
    this->send_all();
    async_loop();
  });
}

