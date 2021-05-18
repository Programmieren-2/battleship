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
using net::Defaults::HOST;
using net::Defaults::PORT;

#include "util.h"
using util::parseArgDesc;

#include "GameServer.h"
using proto::GameServer;

static auto parseArgs(int argc, const char *argv[])
{
    options_description desc("Command line options");
    desc.add_options()
            ("help,h", "Show this page")
            ("address,a", value<string>()->default_value(HOST), "IP address to listen on")
            ("port,p", value<unsigned short>()->default_value(PORT), "Port to listen on");
    return parseArgDesc(argc, argv, desc);
}

int main(int argc, const char *argv[])
{
    auto optArgs = parseArgs(argc, argv);
    if (!optArgs.has_value())
        return 1;

    auto args = optArgs.value();
    auto address = args.at("address").as<string>();
    auto port = args.at("port").as<unsigned short>();

    GameServer server(address, port);
    server.listen();
}