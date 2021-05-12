#include <iostream>
using std::cerr;

#include <string>
using std::string;

#include "GameServer.h"
using proto::GameServer;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cerr << "Must specify hostname and port!\n";
        return 2;
    }

    string host = argv[1];
    string port = argv[2];
    GameServer server(host, static_cast<unsigned short>(stoul(port)));
    server.listen();
}