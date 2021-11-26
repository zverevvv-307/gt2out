#include "gtclient.h"


#include "../gtlan/gteth.h"
#include "dmpacket2.h"
#include "helpers.h"


using namespace std;

GtClient::GtClient( boost::asio::io_service& io_service)
    : isocket_(io_service),
      mcast_(io_service)
{
  this->listen_address_ = boost::asio::ip::address_v4::from_string(Options::instance()->local_address_wan);
  this->listen_endpoint_ = boost::asio::ip::udp::endpoint(this->listen_address_, Options::instance()->remote_port);
  this->isocket_.open(this->listen_endpoint_.protocol());
  this->isocket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
  isocket_.bind(listen_endpoint_);

  if( !mcast_.open(
          [](const std::string& e) { std::cerr<<std::endl<<"MCAST: "<<e<<std::endl; },
          Options::instance()->local_address_gt.empty() ? nullptr : Options::instance()->local_address_gt.c_str(),
          1)
      )
    throw std::runtime_error("GtClient failed.");

  async_receive_from();
}

void GtClient::async_receive_from() {
  isocket_.async_receive_from( boost::asio::buffer(data), sender_endpoint_,
                              [this](const boost::system::error_code& err, size_t bytes_recvd){
                                this->handle_receive_from(err, bytes_recvd);
                              });
}


void GtClient::handle_receive_from(const boost::system::error_code& err, size_t bytes_recvd)
{
  if( !err ) {
    TDatagram2 *dtgrm = reinterpret_cast<TDatagram2*>(&data[0]);
    size_t iln = dtgrm->Size + TDatagram2_HEADER_LN;
    if(iln == bytes_recvd){ // check valid

      TDatagramPacket2& cell = this->get( {dtgrm->Type, dtgrm->Name} );
      if(dtgrm->Size)
        cell.Dtgrm = *dtgrm;

      cell.msec = Vx::timestamp();
      cell.time = Vx::time_t32_now();
      mcast_.blocking_send(cell.Dtgrm);
      cell.tick++;

      cout << setw(4) << bytes_recvd << "; "
           << setw(12) << dtgrm->Type <<"; "
           << setw(12) << dtgrm->Name <<"; "
           << setw(4) << dtgrm->Size << "; "
           << std::endl;
    }
  }
  else
    cerr << err << " " << err.message()<< ". " << __PRETTY_FUNCTION__ << endl;
  async_receive_from();
}
