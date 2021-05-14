//
// Created by rne on 08.05.21.
//

#include <optional>
using std::optional;

#include <stdexcept>
using std::out_of_range;

#include <string>
using std::string;

#include <utility>
using std::move;

#include <vector>
using std::vector;

#include "Coordinate.h"
using models::Coordinate;

#include "PlayerBoard.h"
using models::PlayerBoard;

#include "Ship.h"
using models::Ship;
using models::ShipTypes;

#include "Constants.h"
#include "Server.h"
using net::Server;

#include "Messages.h"
#include "Player.h"

#include "util.h"
using util::contains;
using util::copyString;

#include "GameServer.h"

namespace proto {
    GameServer::GameServer(string const &host, unsigned short port, ShipTypes shipTypes)
            : Server(host, port), players(Players()), shipTypes(move(shipTypes)),
            state(GameState::WAITING_FOR_PLAYERS)
    {}

    GameServer::GameServer(string const &host, unsigned short port)
            : GameServer(host, port, models::Constants::SHIP_TYPES)
    {}

    GameServer::GameServer(ShipTypes shipTypes)
            : Server(), shipTypes(move(shipTypes))
    {}

    GameServer::GameServer()
            : GameServer(models::Constants::SHIP_TYPES)
    {}

    auto GameServer::getPlayer(unsigned long playerId)
    {
        optional<PlayerRef> result;

        for (Player &player : players) {
            if (player.getId() == playerId)
                return result = player;
        }

        return result;
    }

    auto GameServer::getOpponent(unsigned long playerId)
    {
        optional<PlayerRef> result;

        for (Player &player : players) {
            if (player.getId() != playerId)
                return result = player;
        }

        return result;
    }

    string GameServer::processLoginRequest(LoginRequest const &request)
    {
        LoginResponse response;
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

    string GameServer::processMapRequest(MapRequest const &request)
    {
        MapResponse response;
        response.header.playerId = request.header.playerId;
        auto playerId = static_cast<unsigned long>(request.header.playerId);
        auto candidate = request.own ? getPlayer(playerId) : getOpponent(playerId);

        if (!candidate.has_value())
            return serialize(InvalidRequest());

        Player &player = candidate.value();
        PlayerBoard &board = player.getBoard();
        response.width = board.getWidth();
        response.height = board.getHeight();
        string targetBoardMap = board.toString(request.own);
        response.size = static_cast<uint32_t>(targetBoardMap.size());
        string buf = serialize(response);
        buf += targetBoardMap;
        return buf;
    }

    string GameServer::processShipPlacementRequest(ShipPlacementRequest const &request)
    {
        auto candidate = getPlayer(request.header.playerId);

        if (!candidate.has_value())
            return serialize(InvalidRequest());

        Player &player = candidate.value();
        PlayerBoard &board = player.getBoard();
        string type = request.type;

        if (shipTypes.count(type) == 0 || board.hasShip(type))
            return serialize(InvalidRequest());

        unsigned short length = shipTypes.at(type);
        Ship ship(type, Coordinate(request.x, request.y), length, request.orientation);
        ShipPlacementResponse response;
        response.result = board.placeShip(ship);
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
        auto header = deserialize<RequestHeader>(buf, true);
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