#include <iostream>
using std::cerr;
using std::cin;
using std::cout;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Ship.h"
using models::ShipTypes;

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

static void printShipTypes(ShipTypes shipTypes)
{
    cout << "The server has the following ship types:\n";

    for (auto const &[name, size] : shipTypes)
        cout << "* " << name << " (" << size << ")\n";
}

static void printLoginResult(bool accepted)
{
    if (accepted)
        cout << "Server allowed us to login. Yay!\n";
    else
        cerr << "Server denied us to login. Darn it!\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cerr << "Must specify hostname and port!\n";
        return 2;
    }

    string host = argv[1];
    string port = argv[2];

    GameClient client(host, static_cast<unsigned short>(stoul(port)));
    cout << "Enter your name to login or 'ships' to list the ships or 'exit' to exit.\n";

    while (true) {
        cout << "% ";
        string input;
        cin >> input;

        if (checkExit(input))
            return 0;

        if (input == "ships")
            printShipTypes(client.getShipTypes());
        else
            printLoginResult(client.login(input));
    }
}