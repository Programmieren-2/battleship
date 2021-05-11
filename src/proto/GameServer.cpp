//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;

#include <map>
using std::map;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Constants.h"
#include "Server.h"

#include "util.h"
using util::contains;
using util::copyString;

#include "Messages.h"
#include "GameServer.h"

namespace proto {
    GameServer::GameServer(string const &host, unsigned short port, map<string, unsigned short> shipTypes)
            : Server(host, port), shipTypes(shipTypes)
    {}

    GameServer::GameServer(unsigned short port, map<string, unsigned short> shipTypes)
            : Server(port), shipTypes(shipTypes)
    {}

    GameServer::GameServer(string const &host, map<string, unsigned short> shipTypes)
            : Server(host), shipTypes(shipTypes)
    {}

    GameServer::GameServer(map<string, unsigned short> shipTypes)
            : Server(), shipTypes(shipTypes)
    {}

    GameServer::GameServer(string const &host, unsigned short port)
            : GameServer(host, port, models::Constants::shipTypes)
    {}

    GameServer::GameServer(unsigned short port)
            : GameServer(port, models::Constants::shipTypes)
    {}

    GameServer::GameServer(string const &host)
            : GameServer(host, models::Constants::shipTypes)
    {}

    GameServer::GameServer()
            : GameServer(models::Constants::shipTypes)
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

        if (contains(models::Constants::VALID_PLAYER_NAMES, playerName)) {
            cerr << "Player name is whitelisted. Allowing login.\n";
            response.accepted = true;
        } else {
            cerr << "Player name is not whitelisted. Denying login.\n";
        }

        return serialize(response);
    }

    string GameServer::processShipTypesRequest(ShipTypesRequest const &shipTypesRequest)
    {
        ShipTypesResponse shipTypesResponse;
        shipTypesResponse.ships = (uint8_t) shipTypes.size();
        string buf = serialize(shipTypesResponse);
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
        for (auto &[name, size] : shipTypes) {
            ShipType shipType;
            copyString(shipType.name, name, sizeof shipType.name);
            shipType.size = (uint8_t) size;
            buf += serialize(shipType);
        }
    }

    string GameServer::handleRequest(string const &buf)
    {
        RequestHeader header = deserialize<RequestHeader>(buf, true);
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
}