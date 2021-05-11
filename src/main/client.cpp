#include <iostream>
using std::cerr;
using std::cin;
using std::cout;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "util.h"
using util::splitString;

#include "GameClient.h"
using proto::GameClient;

static bool checkExit(string const &command)
{
    if (cin.eof())
        cout << "\n";

    return cin.eof() || command == "exit" || command == "quit";
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cerr << "Must specify hostname and port!\n";
        return 2;
    }

    string host = argv[1];
    string port = argv[2];

    GameClient client(host, stoul(port));
    cout << "Enter your name to login or 'ships' to list the ships or 'exit' to exit.\n";

    while (true) {
        cout << "% ";
        string command;
        cin >> command;

        if (checkExit(command))
            return 0;

        if (command == "ships")
            client.sendShipTypesRequest();
        else
            client.sendLoginRequest(command);
    }
}