//
// Created by rne on 15.05.21.
//

#include <functional>
using std::reference_wrapper;

#include <optional>
using std::optional;

#include <string>
using std::string;

#include <stdexcept>
using std::out_of_range;

#include "Models.h"
#include "Coordinate.h"
using models::Coordinate;

#include "Game.h"
using models::Game;

#include "Sea.h"
using models::PlacementResult;
using models::Sea;

#include "Ship.h"
using models::Ship;
using models::ShipTypes;

#include "OnlinePlayer.h"

#include "util.h"
using util::copyString;

#include "Messages.h"
#include "OnlineGame.h"

namespace proto {
    OnlineGame::OnlineGame(unsigned long id, unsigned short width, unsigned short height)
        : Game(width, height), id(id), state(GameState::INITIAL)
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
            if (player.get().getId() != playerId)
                return result = player;
        }

        return result;
    }

    auto OnlineGame::getPlayer(unsigned long playerId)
    {
        optional<reference_wrapper<OnlinePlayer>> result;

        for (auto &player : getPlayers()) {
            if (player.get().getId() == playerId)
                return result = player;
        }

        return result;
    }

    bool OnlineGame::allShipsPlaced(Sea const &sea)
    {
        ShipTypes shipTypes = getShipTypes();
        return all_of(shipTypes.begin(), shipTypes.end(), [sea] (auto &pair) { return sea.hasShip(pair.first); });
    }

    LoginResponse OnlineGame::processLoginRequest(LoginRequest const &request)
    {
        unsigned long playerId = getPlayers().size() + 1;
        bool success = addPlayer(OnlinePlayer(playerId, request.playerName, makeSea()));
        return LoginResponse(id, success ? playerId : 0, success);
    }

    string OnlineGame::processLoginRequest(string const &buf)
    {
        return serialize(processLoginRequest(deserialize<LoginRequest>(buf)));
    }

    LogoutResponse OnlineGame::processLogoutRequest(LogoutRequest const &request)
    {
        auto player = getPlayer(request.header.playerId);
        if (BOOST_UNLIKELY(!player.has_value()))
            return LogoutResponse(request.header.gameId, request.header.playerId, false);

        removePlayer(player.value());
        return LogoutResponse(request.header.gameId, request.header.playerId, true);
    }

    string OnlineGame::processLogoutRequest(string const &buf)
    {
        return serialize(processLogoutRequest(deserialize<LogoutRequest>(buf)));
    }

    string OnlineGame::processShipTypesRequest(ShipTypesRequest const &request)
    {
        ShipTypesResponse response;
        response.header.playerId = request.header.playerId;
        ShipTypes shipTypes = getShipTypes();
        response.ships = static_cast<uint8_t>(shipTypes.size());
        string buf = serialize(response);

        for (auto &[name, size] : shipTypes) {
            ShipType shipType(name, static_cast<uint8_t>(size));
            buf += serialize(shipType);
        }

        return buf;
    }

    string OnlineGame::processShipTypesRequest(string const &buf)
    {
        return processShipTypesRequest(deserialize<ShipTypesRequest>(buf));
    }

    string OnlineGame::processMapRequest(MapRequest const &request)
    {
        auto playerId = static_cast<unsigned long>(request.header.playerId);
        auto candidate = request.own ? getPlayer(playerId) : getOpponent(playerId);
        if (BOOST_UNLIKELY(!candidate.has_value()))
            throw InvalidRequest(NO_SUCH_PLAYER);

        OnlinePlayer &player = candidate.value();
        Sea &sea = player.getSea();
        string map = sea.toString(request.own);
        MapResponse response(id, player.getId(), sea.getWidth(), sea.getHeight(), static_cast<uint32_t>(map.size()));
        return serialize(response) + map;
    }

    string OnlineGame::processMapRequest(string const &buf)
    {
        try {
            return processMapRequest(deserialize<MapRequest>(buf));
        } catch (InvalidRequest &invalidRequest) {
            return serialize(invalidRequest);
        }
    }

    ShipPlacementResponse OnlineGame::processShipPlacementRequest(ShipPlacementRequest const &request)
    {
        auto candidate = getPlayer(request.header.playerId);
        if (BOOST_UNLIKELY(!candidate.has_value()))
            throw InvalidRequest(NO_SUCH_PLAYER);

        OnlinePlayer &player = candidate.value();
        Sea &sea = player.getSea();
        string type = request.type;
        PlacementResult result;

        ShipTypes shipTypes = getShipTypes();
        if (BOOST_UNLIKELY(shipTypes.count(type) == 0)) {
            result = PlacementResult::INVALID_SHIP_TYPE;
        } else if (BOOST_UNLIKELY(sea.hasShip(type))) {
            result = PlacementResult::ALREADY_PLACED;
        } else {
            Ship ship(type, Coordinate(request.x, request.y), shipTypes.at(type), request.orientation);
            result = sea.placeShip(ship);
        }

        return ShipPlacementResponse(id, player.getId(), result);
    }

    string OnlineGame::processShipPlacementRequest(string const &buf)
    {
        try {
            return serialize(processShipPlacementRequest(deserialize<ShipPlacementRequest>(buf)));
        } catch (InvalidRequest &invalidRequest) {
            return serialize(invalidRequest);
        }
    }

    StatusResponse OnlineGame::processStatusRequest(StatusRequest const &request)
    {
        auto candidate = getPlayer(request.header.playerId);
        if (BOOST_UNLIKELY(!candidate.has_value()))
            throw InvalidRequest(NO_SUCH_PLAYER);

        OnlinePlayer &player = candidate.value();
        return StatusResponse(id, player.getId(), state);
    }

    string OnlineGame::processStatusRequest(string const &buf)
    {
        try {
            return serialize(processStatusRequest(deserialize<StatusRequest>(buf)));
        } catch (InvalidRequest &invalidRequest) {
            return serialize(invalidRequest);
        }
    }

    TurnResponse OnlineGame::processTurnRequest(TurnRequest const &request)
    {
        optional<OnlinePlayerReference> playerRef = getPlayer(request.header.playerId);
        if (!playerRef.has_value())
            throw InvalidRequest(NO_SUCH_PLAYER);

        OnlinePlayer &player = playerRef.value();

        optional<OnlinePlayerReference> opponentRef = getOpponent(request.header.playerId);
        if (!opponentRef.has_value())
            throw InvalidRequest(NO_OPPONENT);

        OnlinePlayer &opponent = opponentRef.value();

        if (!currentPlayer.has_value() || currentPlayer.value() != player)
            throw InvalidRequest(ErrorType::NOT_YOUR_TURN);

        Coordinate target = Coordinate(request.x, request.y);

        Sea &sea = opponent.getSea();
        if (!allShipsPlaced(sea))
            throw InvalidRequest(OPPONENT_NOT_READY);

        currentPlayer = opponent;
        return TurnResponse(id, player.getId(), sea.fireAt(target), sea.allShipsDestroyed());
    }

    string OnlineGame::processTurnRequest(string const &buf)
    {
        try {
            return serialize(processTurnRequest(deserialize<TurnRequest>(buf)));
        } catch (InvalidRequest &invalidRequest) {
            return serialize(invalidRequest);
        }
    }
}