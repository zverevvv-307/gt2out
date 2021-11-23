#include "gtreceivespy.h"


#include "../gtlan/gteth.h"
#include "dmpacket2.h"


using namespace std;

GtReceiveSpy::GtReceiveSpy(
    int r,
    boost::asio::io_service& io_service,
    const std::string& local_interface_name,
    int port)
    : socket(io_service),
      listen_address(boost::asio::ip::address::from_string(local_interface_name)),
      rejim(r)
{
  this->listen_endpoint = Endpoint(this->listen_address, port);
  this->socket.open(this->listen_endpoint.protocol());
  this->socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));

  async_receive_from();
}

GtReceiveSpy::~GtReceiveSpy()
{
  ofs.close();
}


void GtReceiveSpy::handle_receive_from(const boost::system::error_code& err, size_t bytes_recvd)
{
  if(err) {
    cerr << err << " " << err.message()<< ". " << __PRETTY_FUNCTION__ << endl;
    return;
  }


  TDatagram2 *dtg2 = reinterpret_cast<TDatagram2*>(&data[0]);

  time_t now; time(&now);
  tm* t = localtime(&now);
  char buf[64];
  snprintf(buf, sizeof(buf), "%02d:%02d\'%02d %02d-%02d-%04d", t->tm_hour, t->tm_min, t->tm_sec, t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
  string name(&data[4], 12);
  cout << setw(20) << buf <<"; " << setw(4) << bytes_recvd << "; " << setw(12) << name.c_str() << "; ";
  cout << setw(12) << dtg2->Name <<"; " << setw(4) << dtg2->Size << "; " << setw(4) << (int)dtg2->Data[0] << "; ";
  cout << std::endl;


  if(rejim >1 ){
    static int current_day = 0;
    if( current_day != t->tm_yday ){
      current_day = t->tm_yday;
      ofs.close();
      char fnm[64];
      snprintf(fnm, sizeof(fnm), "%03dday.bin", t->tm_yday);
      ofs.open( fnm, ios_base::binary | ios_base::out | ios_base::app );
    }
    ofs.write( (char*)&now, sizeof(now) );
    ofs.write( data.data(), bytes_recvd );
  }

  async_receive_from();
}
