#include "rtask.h"
#include <iostream>
#include <string>

#include "opt.h"
#include "dmpacket2.h"
#include "dmpacket.h"
#include "gtasioipx.h" //только в cpp иначе несоберется с http(((
#include "../gtlan/gteth.h"

using namespace std;

//------------------------------------------------------------------------------
ReceiveTask::ReceiveTask(boost::asio::io_context& ios, FilterPtr f)
    : io_context(ios)
      ,filter_(f)
      ,ipx_( std::make_unique<GtAsioIpxSocket>(ios))
      ,mcast_( std::make_unique<GtEthernet>(ios) )
{
}
ReceiveTask::~ReceiveTask(){
}
//------------------------------------------------------------------------------
bool ReceiveTask::open()
{
  if( !Options::instance()->no_ipx) { // old net
    if( !ipx_->open( true, [](const std::string& e) { std::cerr<<std::endl<<"IPX: "<<e<<std::endl; } ) )
      return false;
    ipx_->start_receiver( [this](const TDatagram& d) { processI(d); } );
  }
  else { // multicast
    if( !mcast_->open([](const std::string& e) { std::cerr<<std::endl<<"MCAST: "<<e<<std::endl; }
                     ,Options::instance()->local_address_gt.empty() ? nullptr : Options::instance()->local_address_gt.c_str()
                     ,1)
        )
      return false;
    mcast_->start_receiver( [this](const TDatagram2& d2, const char* )  { processI2(d2); } );
  }
  return true;
}

void ReceiveTask::processI(const TDatagram& dtg)
{
  if( filter_->sift(dtg) ) {
    count_++;
  }
}

void ReceiveTask::processI2(const TDatagram2& dtg2)
{
  if( filter_->sift2(dtg2) ) {
    count_++;
  }
}





