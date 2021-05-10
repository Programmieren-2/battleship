//
// Created by rne on 08.05.21.
//

#include <algorithm>
using std::find;

#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Net.h"
using net::Socket;

#include "Server.h"
#include "util.h"
using util::contains;

#include "Constants.h"

#include "Packets.h"
#include "GameServer.h"

namespace proto {
    GameServer::GameServer()
            : Server()
    {}

    LoginResponse GameServer::createLoginResponse(bool accepted)
    {
        LoginResponse loginResponse;
        loginResponse.accepted = accepted;
        return loginResponse;
    }

    string GameServer::processLoginRequest(LoginRequest const &loginRequest)
    {
        LoginResponse response;
        string playerName = loginRequest.playerName;
        cerr << "Player '" << playerName << "' wants to log in." << endl;

        if (contains(models::Constants::VALID_PLAYER_NAMES, playerName)) {
            cerr << "Player name is whitelisted. Allowing login." << endl;
            response.accepted = true;
        } else {
            cerr << "Player name is not whitelisted. Denying login." << endl;
        }

        cerr << "Response size: " << sizeof response << endl;
        return serialize(response);
    }

    string GameServer::processShipTypesRequest(ShipTypesRequest const &shipTypesRequest)
    {
        ShipTypesResponse response;
        string buf = serialize(response);
        appendShipTypes(buf);
        return buf;
    }

    string GameServer::processMapRequest(MapRequest const &mapRequest)
    {
        MapResponse response;
        return serialize(response);
    }

    string GameServer::processShipPlacementRequest(ShipPlacementRequest const &shipPlacementRequest)
    {
        ShipPlacementResponse response;
        return serialize(response);
    }

    string GameServer::processStatusRequest(StatusRequest const &statusRequest)
    {
        StatusResponse response;
        return serialize(response);
    }

    string GameServer::processTurnRequest(TurnRequest const &turnRequest)
    {
        TurnResponse response;
        return serialize(response);
    }

    void GameServer::appendShipTypes(string &buf)
    {
        for (auto &[name, size] : models::Constants::shipTypes) {
            ShipType shipType;
            strncpy(shipType.name, name.c_str(), sizeof shipType.name);
            shipType.size = size;
            buf += serialize(shipType);
        }
    }

    string GameServer::processRequest(net::Socket &socket)
    {
        string buf = receive(socket);
        cerr << "Bytes received: " << buf.size() << endl;
        RequestHeader header = deserialize<RequestHeader>(buf, true);
        cerr << "Received request " << header.type << " from player " << header.playerId << "." << endl;
        InvalidRequest invalidRequest;

        switch (header.type) {
            case RequestType::LOGIN_REQUEST:
                return processLoginRequest(deserialize<LoginRequest>(buf));
            case RequestType::SHIP_TYPES_REQUEST:
                return processShipTypesRequest(deserialize<ShipTypesRequest>(buf));
            case RequestType::MAP_REQUEST:
                return processMapRequest(deserialize<MapRequest>(buf));
            case RequestType::SHIP_PLACEMENT_REQUEST:
                return processShipPlacementRequest(deserialize<ShipPlacementRequest>(buf));
            case RequestType::STATUS_REQUEST:
                return processStatusRequest(deserialize<StatusRequest>(buf));
            case RequestType::TURN_REQUEST:
                return processTurnRequest(deserialize<TurnRequest>(buf));
            default:
                return serialize(invalidRequest);
        }
    }

    void GameServer::listen()
    {
        string response;

        while (true) {
            Socket socket = getSocket();
            response = processRequest(socket);
            send(socket, response);
        }
    }
}