#include <iostream>
using std::cout;

#include <optional>
using std::optional;

#include <string>
using std::string;

#include <boost/program_options.hpp>
using boost::program_options::options_description;
using boost::program_options::value;

#include "Net.h"
using net::Defaults::ADDRESS;
using net::Defaults::PORT;

#include "ip.h"
using ip::getAddress;

#include "util.h"
using util::parseArgDesc;

#include "CLIClient.h"
using proto::CLIClient;

static auto parseArgs(int argc, const char *argv[])
{
    options_description desc("Command line options");
    desc.add_options()
            ("help,h", "Show this page")
            ("address,a", value<string>()->default_value(ADDRESS)->value_name("ip_address"),
                    "IP address to connect to")
            ("port,p", value<unsigned short>()->default_value(PORT)->value_name("portnum"),
                    "Port to connect to");
    return parseArgDesc(argc, argv, desc);
}

int main(int argc, const char *argv[])
{
    auto optArgs = parseArgs(argc, argv);
    if (!optArgs.has_value())
        return 1;

    auto args = optArgs.value();
    auto host = args.at("address").as<string>();
    auto address = getAddress(host);
    auto port = args.at("port").as<unsigned short>();

    CLIClient client(address, port);
    client.loop();

    return 0;
}