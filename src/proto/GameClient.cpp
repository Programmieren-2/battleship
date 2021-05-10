//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <map>
using std::map;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Client.h"
using net::Client;

#include "Net.h"
using net::Socket;

#include "util.h"
using util::copyString;

#include "Packets.h"
#include "GameClient.h"

namespace proto {
    GameClient::GameClient(string const &host, unsigned short port)
            : Client(host, port), playerId(0)
    {}

    GameClient::GameClient(string const &host)
            : Client(host), playerId(0)
    {}

    GameClient::GameClient(unsigned short port)
            : Client(port), playerId(0)
    {}

    GameClient::GameClient()
            : Client(), playerId(0)
    {}

    LoginRequest GameClient::createLoginRequest(const std::string &playerName)
    {
        LoginRequest loginRequest;
        loginRequest.header.playerId = playerId;
        size_t maxPlayerNameSize = sizeof loginRequest.playerName;

        if (playerName.size() > maxPlayerNameSize)
            cerr << "Player name exceeds " << maxPlayerNameSize << " bytes, so it will be truncated." << endl;

        copyString(loginRequest.playerName, playerName, maxPlayerNameSize);
        return loginRequest;
    }

    ShipTypesRequest GameClient::createShipTypesRequest()
    {
        ShipTypesRequest shipTypesRequest;
        return shipTypesRequest;
    }

    void GameClient::processShipTypesResponse(string &buf)
    {
        ShipTypesResponse shipTypesResponse = deserialize<ShipTypesResponse>(buf, true);
        size_t offset = sizeof shipTypesResponse;
        ShipType shipType;
        size_t shipTypeSize = sizeof shipType;
        map<string, unsigned short> shipTypes;
        string shipTypeBuf;

        for (unsigned short i = 0; i < shipTypesResponse.ships; i++) {
            shipTypeBuf = buf.substr(offset, shipTypeSize);
            shipType = deserialize<ShipType>(shipTypeBuf, true);
            shipTypes[shipType.name] = shipType.size;
            offset += shipTypeSize;
        }

        cout << "The server has the following ship types:" << endl;

        for (auto &[name, size] : shipTypes)
            cout << "* " << name << " (" << size << ")" << endl;
    }

    void GameClient::processResponse(string &buf)
    {
        ResponseHeader header = deserialize<ResponseHeader>(buf, true);

        switch (header.type) {
            case ResponseType::LOGIN_RESPONSE:
                processLoginResponse(deserialize<LoginResponse>(buf));
                break;
            case ResponseType::SHIP_TYPES_RESPONSE:
                processShipTypesResponse(buf);
                break;
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

    void GameClient::sendShipTypesRequest()
    {
        ShipTypesRequest shipTypesRequest = createShipTypesRequest();
        string response = communicate(serialize(shipTypesRequest));
        processResponse(response);
    }
}