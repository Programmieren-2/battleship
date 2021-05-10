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
    GameClient::GameClient(unsigned long playerId)
            : playerId(playerId)
    {}

    GameClient::GameClient()
            : GameClient(0)
    {}

    LoginRequest GameClient::createLoginRequest(const std::string &playerName)
    {
        LoginRequest loginRequest;
        loginRequest.header.playerId = playerId;
        size_t maxPlayerNameSize = sizeof loginRequest.playerName;

        if (playerName.size() > maxPlayerNameSize)
            cerr << "Player name exceeds " << maxPlayerNameSize << " bytes, so it will be truncated." << endl;

        strncpy(loginRequest.playerName, playerName.c_str(), maxPlayerNameSize);
        return loginRequest;
    }

    void GameClient::processResponse(string &buf)
    {
        ResponseHeader header = deserialize<ResponseHeader>(buf, true);

        switch (header.type) {
            case ResponseType::LOGIN_RESPONSE:
                processLoginResponse(deserialize<LoginResponse>(buf));
        }
    }

    void GameClient::processLoginResponse(LoginResponse const &loginResponse)
    {
        if (loginResponse.accepted)
            cout << "Server allowed us to login. Yay!" << endl;
        else
            cerr << "Server denied us to login. Darn it!" << endl;
    }

    bool GameClient::sendLoginRequest(string const &name)
    {
        LoginRequest loginRequest = createLoginRequest(name);
        string response = communicate(serialize(loginRequest));
        processResponse(response);
        return true;
    }
}