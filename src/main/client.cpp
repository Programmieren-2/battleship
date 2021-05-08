#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "Client.h"
using net::Client;

int main()
{
    Client client;

    while (true) {
        string message;
        cout << "Enter message: ";
        cin >> message;
        cout << "Sending message '" << message << "' to server." << endl;
        string response = client.communicate(message);
        cout << "Server responded with: " << response << endl;

        if (message == "terminate" || message == "exit")
            break;
    }

    return 0;
}