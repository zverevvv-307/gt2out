#ifndef OPT_H
#define OPT_H

#include <string>
#include <boost/program_options.hpp>

//------------------------------------------------------------------------------
namespace po = boost::program_options;

class Options
{
    Options ();
    ~Options () = default;

public:

    static Options *instance ();
    po::variables_map vm;
    int parse_args (int argc, char *argv[]);

    std::string config_file;
    bool nnov_nocodec;
    bool is_ipx;

    std::string local_address_gt;
    std::string local_address_kit;
    std::string remote_address;
    int remote_port;

    int queue_watermark;
    std::string log_level;
    int  verbose;

    int spy;
};
//------------------------------------------------------------------------------
#define MKOK "mkokk"




#endif /* OPTIONS_H */
