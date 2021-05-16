#include <iostream>
using std::cout;

#include <optional>
using std::optional;

#include <string>
using std::string;

#include <boost/program_options.hpp>

#include "Net.h"

#include "GameServer.h"
using proto::GameServer;

static auto parseArgs(int argc, const char *argv[])
{
    namespace args = boost::program_options;
    args::options_description desc("Command line options");
    desc.add_options()
        ("help", "Show this page")
        ("address", args::value<string>()->default_value(net::Defaults::HOST), "IP address to listen on")
        ("port", args::value<unsigned short>()->default_value(net::Defaults::PORT), "Port to listen on")
    ;

    args::variables_map varMap;
    args::store(args::parse_command_line(argc, argv, desc), varMap);
    args::notify(varMap);
    optional<args::variables_map> result;

    if (varMap.count("help")) {
        cout << desc << "\n";
        return result;
    }

    return result = varMap;
}

int main(int argc, const char *argv[])
{
    auto parsedArgs = parseArgs(argc, argv);
    if (!parsedArgs.has_value())
        return 1;

    auto args = parsedArgs.value();
    auto address = args.at("address").as<string>();
    auto port = args.at("port").as<unsigned short>();

    GameServer server(address, port);
    server.listen();
}