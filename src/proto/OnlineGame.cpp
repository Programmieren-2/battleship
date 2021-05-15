//
// Created by rne on 15.05.21.
//

#include <optional>
using std::optional;

#include <string>
using std::string;

#include <stdexcept>
using std::out_of_range;

#include "Models.h"
#include "Coordinate.h"
using models::Coordinate;

#include "Sea.h"
using models::PlacementResult;
using models::Sea;

#include "Ship.h"
using models::Ship;
using models::ShipTypes;

#include "util.h"
using util::copyString;

#include "Messages.h"
#include "OnlineGame.h"

namespace proto {
    OnlineGame::OnlineGame(unsigned long id, unsigned short width, unsigned short height)
            : Game(width, height), id(id)
    {}

    Sea OnlineGame::makeSea() const
    {
        return Sea(getWidth(), getHeight());
    }

    unsigned long OnlineGame::getId() const
    {
        return id;
    }

    auto OnlineGame::getOpponent(unsigned long playerId)
    {
        optional<OnlinePlayerReference> result;

        for (auto &player : getPlayers()) {
            if (player.getId() != playerId)
                return result = player;
        }

        return result;
    }

    auto OnlineGame::getPlayer(unsigned long playerId)
    {
        optional<OnlinePlayerReference> result;

        for (auto &player : getPlayers()) {
            if (player.getId() == playerId)
                return result = player;
        }

        return result;
    }

    string OnlineGame::processLoginRequest(LoginRequest const &request)
    {
        LoginResponse response;
        string playerName = request.playerName;

        unsigned long playerId = getPlayers().size() + 1;
        Sea sea = makeSea();
        bool success = addPlayer(OnlinePlayer(playerId, playerName, sea));

        if (success) {
            response.header.playerId = playerId;
            response.accepted = true;
        }

        return serialize(response);
    }

    string OnlineGame::processShipTypesRequest(ShipTypesRequest const &request) const
    {
        ShipTypesResponse response;
        response.header.playerId = request.header.playerId;
        ShipTypes shipTypes = getShipTypes();
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

    string OnlineGame::processMapRequest(MapRequest const &request)
    {
        MapResponse response;
        response.header.playerId = request.header.playerId;
        auto playerId = static_cast<unsigned long>(request.header.playerId);
        auto candidate = request.own ? getPlayer(playerId) : getOpponent(playerId);

        if (BOOST_UNLIKELY(!candidate.has_value()))
            return serialize(InvalidRequest());

        OnlinePlayer &player = candidate.value();
        Sea &sea = player.getSea();
        response.width = sea.getWidth();
        response.height = sea.getHeight();
        string map = sea.toString(request.own);
        response.size = static_cast<uint32_t>(map.size());
        string buf = serialize(response);
        buf += map;
        return buf;
    }

    string OnlineGame::processShipPlacementRequest(ShipPlacementRequest const &request)
    {
        auto candidate = getPlayer(request.header.playerId);

        if (BOOST_UNLIKELY(!candidate.has_value()))
            return serialize(InvalidRequest());

        OnlinePlayer &player = candidate.value();
        Sea &sea = player.getSea();
        string type = request.type;
        ShipPlacementResponse response;
        ShipTypes shipTypes = getShipTypes();

        if (BOOST_UNLIKELY(shipTypes.count(type) == 0)) {
            response.result = PlacementResult::INVALID_SHIP_TYPE;
            return serialize(response);
        }

        if (BOOST_UNLIKELY(sea.hasShip(type))) {
            response.result = PlacementResult::ALREADY_PLACED;
            return serialize(response);
        }

        Ship ship(type, Coordinate(request.x, request.y), shipTypes.at(type), request.orientation);
        response.result = sea.placeShip(ship);
        return serialize(response);
    }

    string OnlineGame::processStatusRequest(StatusRequest const &statusRequest) const
    {
        StatusResponse response;
        return serialize(response);
    }

    string OnlineGame::processTurnRequest(TurnRequest const &turnRequest) const
    {
        TurnResponse response;
        return serialize(response);
    }
}