#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "GameClient.h"
using proto::GameClient;

int main()
{
    GameClient client;
    cout << "Enter your name to login or 'ships' to list the ships: ";
    string name;
    cin >> name;

    if (name == "ships")
        client.sendShipTypesRequest();
    else
        client.sendLoginRequest(name);

    return 0;
}