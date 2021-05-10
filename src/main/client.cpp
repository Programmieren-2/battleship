#include <iostream>
using std::cerr;
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "util.h"
using util::splitString;

#include "GameClient.h"
using proto::GameClient;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cerr << "Must specify hostname and port!" << endl;
        return 2;
    }

    string host = argv[1];
    string port = argv[2];

    GameClient client(host, stoul(port));
    cout << "Enter your name to login or 'ships' to list the ships or 'exit' to exit." << endl;

    while (true) {
        cout << "% ";
        string command;
        cin >> command;

        if (command == "exit")
            return 0;

        if (command == "ships")
            client.sendShipTypesRequest();
        else
            client.sendLoginRequest(command);
    }
}