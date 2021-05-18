#include <iostream>
using std::cout;

#include <optional>
using std::optional;

#include <string>
using std::string;

#include <boost/program_options.hpp>

#include "Net.h"

#include "CLIClient.h"
using proto::CLIClient;

namespace args = boost::program_options;

static auto parseArgs(int argc, const char *argv[])
{
    args::options_description desc("Command line options");
    desc.add_options()
        ("help", "Show this page")
        ("address", args::value<string>()->default_value(net::Defaults::HOST), "IP address to connect to")
        ("port", args::value<unsigned short>()->default_value(net::Defaults::PORT), "Port to connect to")
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

    CLIClient client(address, port);
    client.loop();

    return 0;
}