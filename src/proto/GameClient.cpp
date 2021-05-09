//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "Client.h"
using net::Client;

#include "Net.h"
using net::Socket;

#include "Packets.h"

#include "GameClient.h"

namespace proto {
    void GameClient::processResponse(string &buf)
    {
        ResponseHeader header = deserialize<ResponseHeader>(buf, true);
        cerr << "Received response " << header.type << "." << endl;

        switch (header.type) {
            case ResponseType::LOGIN_RESPONSE:
                cerr << "Got valid login response." << endl;
                processLoginResponse(deserialize<LoginResponse>(buf));
        }
    }

    void GameClient::processLoginResponse(LoginResponse const &loginResponse)
    {
        LoginResponse response = loginResponse;
        cout << "Server response: " << response.accepted << endl;
    }

    bool GameClient::sendLoginRequest(string const &name)
    {
        LoginRequest loginRequest;
        loginRequest.header.playerId = 42;
        strncpy(loginRequest.playerName, name.c_str(), 32);
        string response = communicate(serialize(loginRequest));
        processResponse(response);
        return true;
    }
}