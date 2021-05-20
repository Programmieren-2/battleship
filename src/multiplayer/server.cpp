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

#include "util.h"
using util::getAddress;
using util::parseArgDesc;

#include "GameServer.h"
using proto::GameServer;

static auto parseArgs(int argc, const char *argv[])
{
    options_description desc("Command line options");
    desc.add_options()
            ("help,h", "Show this page")
            ("address,a", value<string>()->default_value(ADDRESS)->value_name("ip_address"),
                    "IP address to listen on")
            ("port,p", value<unsigned short>()->default_value(PORT)->value_name("portnum"),
                    "Port to listen on");
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

    GameServer server(address, port);
    server.listen();
}