#include "opt.h"

#include "all_std.h"

using namespace std;
//------------------------------------------------------------------------------
Options * Options::instance ()
{
    static Options singleton;
    return &singleton;
}
//------------------------------------------------------------------------------
Options::Options ()
    :
      nnov_nocodec(false),
      is_ipx(false),
      local_address_gt("0.0.0.0"),
      local_address_kit("0.0.0.0"),
      remote_address("127.0.0.1"),
      remote_port(30001),
      queue_watermark(1024),
      log_level("info"),
      verbose(0),
      spy(0)
{
}
//------------------------------------------------------------------------------
int Options::parse_args (int argc, char *argv[])
{
    po::options_description opts("Options");
    opts.add_options()
        ("help,h", "produce help message")
            ("config-file,c", po::value< std::string >(), "input configuration")
                ("remote-address,r", po::value< std::string >(), "remote address")
                    ("local-address-kit,l", po::value< std::string >(), "interface to bind local socket for kit")
                        ("local-address-gt,g", po::value< std::string >(), "interface to bind local gt socket")
                            ("ipx", "open ipx socket")
                                ("receive-spy,s", po::value< int >(), "receive spy object.")
                                    ("verbose,v", po::value< int >(&verbose)->default_value(0), "verbose")
        ;
    po::options_description hidden("Configuration options");
    hidden.add_options()
        ("log-level", po::value< std::string >(), "log level.")
            ("remote-port,p", po::value< int >(), "remote port.")
                ("queue-watermark,q", po::value< int >(), "queue watermark")
                    ("nnov-nocodec", "n.novgorod ipx protokol")
        ;
    po::options_description hidden2("Hidden options");
    hidden2.add_options()
        ("hh", "produce more help message")
            (MKOK, "monki")
        ;

    po::options_description all_options;
    all_options.add(opts).add(hidden).add(hidden2);

    po::positional_options_description p;
    p.add("config-file", 1);

    po::store(po::command_line_parser(argc, argv).options(all_options).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << opts << "\n";
        return 0;
    }
    if (vm.count("hh")) {
        std::cout << opts << hidden << "\n";
        return 0;
    }

    if (vm.count("log-level"))
        log_level = vm["log-level"].as< std::string  >();
    else log_level = "info";

    if (vm.count("remote-address"))
        remote_address = vm["remote-address"].as< std::string >();
    std::cout << "remote-address: "<< remote_address.c_str() << "\n";

    if (vm.count("remote-port"))
        remote_port = vm["remote-port"].as< int >();
    std::cout << "remote-port: "<< remote_port << "\n";

    if (vm.count("local-address-kit"))
        local_address_kit = vm["local-address-kit"].as< std::string >();
    std::cout << "local-address-kit: "<< local_address_kit.c_str() << "\n";

    if (vm.count("local-address-gt"))
        local_address_gt = vm["local-address-gt"].as< std::string >();
    std::cout << "local-address-gt: "<< local_address_gt.c_str() << "\n";

    if (vm.count("queue-watermark"))
        queue_watermark = vm["queue-watermark"].as< int >();
    std::cout << "queue-watermark: "<< queue_watermark << "\n";

    if (vm.count("ipx"))
        is_ipx = true;
    std::cout << "is ipx: " << is_ipx << "\n";

    if (vm.count("nnov-nocodec"))
        nnov_nocodec = true;
    //    std::cout << "nnov-nocodec: " << int(nnov_nocodec) << "\n";

    if (vm.count("config-file"))
        config_file = vm["config-file"].as< std::string >();
    else {
        std::cout << endl << opts << endl;
        return -1;
    }
    std::cout << "config-file: "<< config_file.c_str() << "\n";


    return 1;
}
//------------------------------------------------------------------------------

