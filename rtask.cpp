#include "rtask.h"
#include "stask.h"

#include <iostream>
#include <string>

#include "opt.h"
#include "dmpacket2.h"
#include "dmpacket.h"

using namespace std;

//------------------------------------------------------------------------------
bool ReceiveTask::open()
{
    if ( !tsfilter_.open(Options::instance()->config_file.c_str()) ) {
        io_context.post(
           [](){
            clog << "!!! Found damage configurations." << endl
                 << "!!! Restore the configuration from the distribution or backup." << endl;
            throw std::runtime_error("Bad config file.");
           }
        );
        return false;
    }

    if(Options::instance()->is_ipx) { // old net
        if( !ipx_.open(!Options::instance()->nnov_nocodec,
                      [this](const std::string& e) { std::cerr<<"IPX: "<<e<<std::endl; this->errors_++; }
                     )
          )
            return false;
            ipx_.start_receiver(
                      [this](const TDatagram& d) { processI(d); }
            );
    }
    else { // base multicast
        if( !mcast_.open([this](const std::string& e) { std::cerr<<"MCAST: "<<e<<std::endl; this->errors_++; }
                         ,Options::instance()->local_address_gt.empty() ? nullptr : Options::instance()->local_address_gt.c_str()
                                                          )
                      )
            return false;

        mcast_.start_receiver(
            [this](const TDatagram2& d2, const char* )  { processI2(d2); }
        );
    }
    return true;
}

void ReceiveTask::processI(const TDatagram& dtg)
{
    count_++;
    if( tsfilter_.sift(dtg, data_) ) {
        if ( ! queue.push(data_) ) {
            ++errors_;
        }
    }
}

void ReceiveTask::processI2(const TDatagram2& dtg2)
{
    count_++;
    if( tsfilter_.sift2(dtg2, data_) ) {
        if ( ! queue.push(data_) ) {
            ++errors_;
        }
    }
}





