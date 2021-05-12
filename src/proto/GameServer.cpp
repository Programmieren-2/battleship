//
// Created by rne on 08.05.21.
//

#include <memory>
using std::unique_ptr;

#include <stdexcept>
using std::out_of_range;

#include <string>
using std::string;

#include <utility>
using std::move;

#include <vector>
using std::vector;

#include "Ship.h"
using models::ShipTypes;

#include "util.h"
using util::contains;
using util::copyString;

#include "PlayerBoard.h"
using models::PlayerBoard;

#include "Constants.h"
#include "Server.h"
using net::Server;

#include "Messages.h"
#include "NoSuchPlayer.h"
#include "Player.h"

#include "GameServer.h"

namespace proto {
    GameServer::GameServer(string const &host, unsigned short port, ShipTypes shipTypes)
            : Server(host, port), players(Players()), shipTypes(move(shipTypes)),
            state(GameState::WAITING_FOR_PLAYERS)
    {}

    GameServer::GameServer(string const &host, unsigned short port)
            : GameServer(host, port, models::Constants::shipTypes)
    {}

    GameServer::GameServer(ShipTypes shipTypes)
            : Server(), shipTypes(move(shipTypes))
    {}

    GameServer::GameServer()
            : GameServer(models::Constants::shipTypes)
    {}

    Player GameServer::getPlayer(unsigned long playerId) const
    {
        for (Player const &player : players) {
            if (player.getId() == playerId)
                return player;
        }

        throw NoSuchPlayer(playerId);
    }

    string GameServer::processLoginRequest(LoginRequest const &request)
    {
        LoginResponse response;
        response.header.playerId = request.header.playerId;
        string playerName = request.playerName;

        if (players.size() < 2) {
            players.push_back(Player(players.size() + 1, playerName));
            response.header.playerId = players.size();
            response.accepted = true;
        }

        return serialize(response);
    }

    string GameServer::processShipTypesRequest(ShipTypesRequest const &request) const
    {
        ShipTypesResponse response;
        response.header.playerId = request.header.playerId;
        response.ships = static_cast<uint8_t>(shipTypes.size());
        string buf = serialize(response);

        for (auto &[name, size] : shipTypes) {
            ShipType shipType;
            copyString(shipType.name, name, sizeof shipType.name);
            shipType.size = static_cast<uint8_t>(size);
            buf += serialize(shipType);
        }

        return buf;
    }

    string GameServer::processMapRequest(MapRequest const &request) const
    {
        MapResponse response;
        response.header.playerId = request.header.playerId;
        auto playerId = static_cast<unsigned long>(request.header.playerId);
        Player otherPlayer;

        try {
            otherPlayer = Player(getPlayer(playerId));
        } catch (NoSuchPlayer&) {
            return serialize(InvalidRequest());
        }

        PlayerBoard targetBoard = otherPlayer.getBoard();
        response.width = targetBoard.getWidth();
        response.height = targetBoard.getHeight();
        string targetBoardMap = targetBoard.toString();
        response.size = static_cast<uint32_t>(targetBoardMap.size());
        string buf = serialize(response);
        buf += targetBoardMap;
        return buf;
    }

    string GameServer::processShipPlacementRequest(ShipPlacementRequest const &request) const
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