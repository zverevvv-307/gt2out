#include <iostream>
#include <iomanip>
#include <string>


#include "asio_pch.h"
#include "gtclient.h"
#include "gtserver.h"
#include "opt.h"


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

        std::shared_ptr<GtServer> server;
        std::shared_ptr<GtClient> client;
        if( !Options::instance()->config_file.empty())
          server = std::make_shared<GtServer>( e.GetIOContext() );
        else
          client = std::make_shared<GtClient>( e.GetIOContext() );

        e.Run(3); //!!blocking send
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
    catch (std::exception& e) {
        std::cerr<<"\nmain->std::exception: "<<e.what()<<std::endl;
    }
    catch (...) {
        std::cerr<<"\nmain->catch (...) ...\n";
    }

    return 0;
}
