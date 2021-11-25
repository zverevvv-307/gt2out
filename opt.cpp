#include "opt.h"

#include "all_std.h"

using namespace std;



int Options::parse_args (int argc, char *argv[])
{
    po::options_description opts("Options");
    opts.add_options()
        ("help,h", "produce help message")
            ("config-file,c", po::value< std::string >(), "input configuration")
                ("remote-address,r", po::value< std::string >(), "remote address")
                    ("local-address-wan,l", po::value< std::string >(), "interface to bind local socket for WAN")
                        ("local-address-gt,g", po::value< std::string >(), "interface to bind local gt socket")
                            ("no-ipx", "dont open ipx socket")
        ("no-webui", "dont open Web UI")
        ;
    po::options_description hidden("Configuration options");
    hidden.add_options()
        ("hh", "produce more help message")
        ("timeout-sec", po::value< int >(&timeout_sec)->default_value(5), "sendloop timeout sec.")
            ("remote-port,p", po::value< int >(&remote_port)->default_value(43210), "remote port.")
        ;

    po::options_description all_options;
    all_options.add(opts).add(hidden);

    po::positional_options_description p;
    p.add("config-file", 1);

    po::store(po::command_line_parser(argc, argv).options(all_options).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << opts << std::endl;
        return 0;
    }
    if (vm.count("hh")) {
        std::cout << opts << hidden << std::endl;
        return 0;
    }

    std::cout << std::endl << "Config:" << std::endl;

    if (vm.count("remote-address"))
        remote_address = vm["remote-address"].as< std::string >();
    std::cout << "remote-address: "<< remote_address.c_str() << std::endl;
    std::cout << "remote-port: "<< remote_port << std::endl;

    if (vm.count("local-address-wan"))
        local_address_wan = vm["local-address-wan"].as< std::string >();
    std::cout << "local-address-wan: "<< local_address_wan.c_str() << std::endl;

    if (vm.count("local-address-gt"))
        local_address_gt = vm["local-address-gt"].as< std::string >();
    std::cout << "local-address-gt: "<< local_address_gt.c_str() << std::endl;

    if (vm.count("no-ipx"))
      no_ipx = true;
    std::cout << "no ipx: " << no_ipx << std::endl;

    if (vm.count("no-webui"))
      no_webui = true;

    if (vm.count("config-file")){
      config_file = vm["config-file"].as< std::string >();
      std::cout << "config-file: "<< config_file.c_str() << std::endl;
    }
    else
      std::cout << "rejime: Client." << std::endl;
    std::cout << std::endl;


    return 1;
}
//------------------------------------------------------------------------------

