#pragma once

#include <string>
#include <boost/program_options.hpp>


namespace po = boost::program_options;

class Options
{
  po::variables_map vm;
public:

  static Options *instance (){
    static Options singleton;
    return &singleton;
  }
  int parse_args (int argc, char *argv[]);

  std::string config_file;
  bool no_ipx = false;
  std::string local_address_gt = "0.0.0.0";
  std::string local_address_wan = "0.0.0.0";
  std::string remote_address = "127.0.0.1";
  int remote_port = 43210;
  int timeout_sec = 7;

};
