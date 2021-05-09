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
    cout << "Enter your name: ";
    string name;
    cin >> name;
    client.sendLoginRequest(name);
    return 0;
}