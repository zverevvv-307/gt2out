#include "gtclient.h"


#include "../gtlan/gteth.h"
#include "dmpacket2.h"


using namespace std;

GtClient::GtClient( boost::asio::io_service& io_service)
    : socket(io_service),
      listen_address( boost::asio::ip::address::from_string(Options::instance()->local_address_wan) ),
      mcast_(io_service)
{
  this->listen_endpoint = Endpoint(this->listen_address, Options::instance()->remote_port);
  this->socket.open(this->listen_endpoint.protocol());
  this->socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));

  if( !mcast_.open(
          [](const std::string& e) { std::cerr<<"MCAST: "<<e<<std::endl; },
          Options::instance()->local_address_gt.empty() ? nullptr : Options::instance()->local_address_gt.c_str()
          )
      )
    throw std::runtime_error("GtClient failed.");


  async_receive_from();
}

void GtClient::async_receive_from() {
  socket.async_receive_from( boost::asio::buffer(data), sender_endpoint,
                            [this](const boost::system::error_code& err, size_t bytes_recvd){
                              this->handle_receive_from(err, bytes_recvd);
                            });
}


void GtClient::handle_receive_from(const boost::system::error_code& err, size_t bytes_recvd)
{
  if(err) {
    cerr << err << " " << err.message()<< ". " << __PRETTY_FUNCTION__ << endl;
    return;
  }

  TDatagram2 *dtg2 = reinterpret_cast<TDatagram2*>(&data[0]);
  mcast_.blocking_send(*dtg2);
  async_receive_from();

  cout <<setw(4) << bytes_recvd << "; "
       << setw(12) << dtg2->Name <<"; "
       << setw(4) << dtg2->Size << "; "
       << setw(4) << (int)dtg2->Data[0]
       << ";\n";
}
