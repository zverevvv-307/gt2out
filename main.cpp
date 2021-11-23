#include <iostream>
#include <iomanip>
#include <string>

#include "opt.h"

#include "rtask.h"
#include "stask.h"
#include "statist.h"
#include "gttsfilter.h"
#include "gtreceivespy.h"
#include "asio_pch.h"


//using namespace std;
using namespace boost::asio;

int main(int argc, char *argv[])
{
#ifdef GIT_REVISION
    std::cout << "Version: " << GIT_REVISION << std::endl;
#endif
    try {
        if( Options::instance()->parse_args(argc, argv) <= 0 )
            return 0;

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        asio_lib::Executor e;
        e.OnWorkerThreadError = [](io_context&, boost::system::error_code ec) {
            std::cerr<<"\n!!! error (asio): "<<ec.message();
        };
        e.OnWorkerThreadException = [](io_context&, const std::exception &ex) {
            std::cerr<<"\n!!! exception (asio): "<<ex.what();
        };
        e.AddCtrlCHandling();


        //spy ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if(Options::instance()->spy){
          GtReceiveSpy r( Options::instance()->spy
                         , e.GetIOContext()
                         , Options::instance()->local_address_kit
                         , Options::instance()->remote_port);
          e.Run(1);
          return 0;
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


        GtDtgrmQueue queue( Options::instance()->queue_watermark );

        SenderTask  stask(e.GetIOContext(), queue);
        if( !stask.open() ) {
            return 1;
        }

        ReceiveTask rtask(e.GetIOContext(), queue);
        if( !rtask.open() ) {
            return 1;
        }

        Statistics statist( e.GetIOContext() );
        statist.data_.push_back(&queue.count_);//0 не нужно чистить
        statist.data_.push_back(&rtask.count_);//1
        statist.data_.push_back(&rtask.errors_);//2
        statist.data_.push_back(&stask.count_);//3
        statist.data_.push_back(&stask.errors_);//4
        statist.data_.push_back(&stask.size_);//5

        e.Run(1);
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    catch (std::exception& e) {
        std::cerr<<"\nmain->std::exception: "<<e.what()<<"\n";
    }
    catch (...) {
        std::cerr<<"\nmain->catch (...) ...\n";
    }

    return 0;
}
//------------------------------------------------------------------------------
