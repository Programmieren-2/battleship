//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Ship.h"
using models::ShipTypes;

#include "Constants.h"
#include "Server.h"

#include "util.h"
using util::contains;
using util::copyString;

#include "Messages.h"
#include "GameServer.h"

namespace proto {
    GameServer::GameServer(string const &host, unsigned short port, ShipTypes shipTypes)
            : Server(host, port), shipTypes(shipTypes)
    {}

    GameServer::GameServer(string const &host, unsigned short port)
            : GameServer(host, port, models::Constants::shipTypes)
    {}

    GameServer::GameServer(ShipTypes shipTypes)
            : Server(), shipTypes(shipTypes)
    {}

    GameServer::GameServer()
            : GameServer(models::Constants::shipTypes)
    {}

    LoginResponse GameServer::createLoginResponse(bool accepted) const
    {
        LoginResponse loginResponse;
        loginResponse.accepted = accepted;
        return loginResponse;
    }

    string GameServer::processLoginRequest(LoginRequest const &loginRequest) const
    {
        string playerName = loginRequest.playerName;

        if (contains(models::Constants::VALID_PLAYER_NAMES, playerName))
            return serialize(createLoginResponse(true));

        return serialize(createLoginResponse(false));
    }

    string GameServer::processShipTypesRequest(ShipTypesRequest const &shipTypesRequest) const
    {
        ShipTypesResponse shipTypesResponse;
        shipTypesResponse.ships = static_cast<uint8_t>(shipTypes.size());
        string buf = serialize(shipTypesResponse);

        for (auto &[name, size] : shipTypes) {
            ShipType shipType;
            copyString(shipType.name, name, sizeof shipType.name);
            shipType.size = static_cast<uint8_t>(size);
            buf += serialize(shipType);
        }

        return buf;
    }

    string GameServer::processMapRequest(MapRequest const &mapRequest) const
    {
        MapResponse response;
        return serialize(response);
    }

    string GameServer::processShipPlacementRequest(ShipPlacementRequest const &shipPlacementRequest) const
    {
        ShipPlacementResponse response;
        return serialize(response);
    }

    string GameServer::processStatusRequest(StatusRequest const &statusRequest) const
    {
        StatusResponse response;
        return serialize(response);
    }

    string GameServer::processTurnRequest(TurnRequest const &turnRequest) const
    {
        TurnResponse response;
        return serialize(response);
    }

    string GameServer::handleRequest(string const &buf) const
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