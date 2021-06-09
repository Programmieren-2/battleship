#include <iostream>
using std::cout;

#include <optional>
using std::optional;

#include <string>
using std::string;

#include <boost/program_options.hpp>
using boost::program_options::options_description;
using boost::program_options::value;

#include "ip.h"
using ip::getAddress;

#include "util.h"
using util::parseArgDesc;

#include "GameServer.h"
using multiplayer::GameServer;

#include "Defaults.h"

static auto parseArgs(int argc, const char *argv[])
{
    options_description desc("CommandCode line options");
    desc.add_options()
            ("help,h", "Show this page")
            ("address,a", value<string>()
                ->default_value(multiplayer::Defaults::ADDRESS)
                ->value_name("ip_address"),
                    "IP address to listen on")
            ("port,p", value<unsigned short>()
                ->default_value(multiplayer::Defaults::PORT)
                ->value_name("portnum"),
                    "Port to listen on");
    return parseArgDesc(argc, argv, desc);
}

int main(int argc, const char *argv[])
{
    auto optArgs = parseArgs(argc, argv);
    if (!optArgs)
        return 1;

    auto args = *optArgs;
    auto host = args.at("address").as<string>();
    auto address = getAddress(host);
    auto port = args.at("port").as<unsigned short>();

    GameServer server(address, port);
    server.listen();
}