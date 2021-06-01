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

    optional<OnlinePlayerReference> OnlineGame::getOpponent(unsigned long playerId) const
    {
        optional<OnlinePlayerReference> result;

        for (auto &player : getPlayers()) {
            if (player.getId() != playerId)
                return result = player;
        }

        return result;
    }

    optional<OnlinePlayerReference> OnlineGame::getOpponent(unsigned long playerId)
    {
        optional<OnlinePlayerReference> result;

        for (auto &player : getPlayers()) {
            if (player.get().getId() != playerId)
                return result = player;
        }

        return result;
    }

    optional<OnlinePlayerReference> OnlineGame::getPlayer(unsigned long playerId) const
    {
        optional<OnlinePlayerReference> result;

        for (auto &player : getPlayers()) {
            if (player.getId() == playerId)
                return result = player;
        }

        return result;
    }

    optional<OnlinePlayerReference> OnlineGame::getPlayer(unsigned long playerId)
    {
        optional<OnlinePlayerReference> result;

        for (auto &player : getPlayers()) {
            if (player.get().getId() == playerId)
                return result = player;
        }

        return result;
    }

    bool OnlineGame::allShipsPlaced(Sea const &sea) const
    {
        ShipTypes availableShipTypes = getShipTypes();
        return all_of(availableShipTypes.begin(), availableShipTypes.end(), [sea] (auto &pair) {
            return sea.hasShip(pair.first);
        });
    }

    LoginResponse OnlineGame::processLoginRequest(LoginRequest const &request)
    {
        auto playerId = static_cast<uint32_t>(getPlayers().size() + 1);
        auto newPlayer = OnlinePlayer(playerId, request.playerName, makeSea());
        bool success = addPlayer(newPlayer);
        if (success && !currentPlayer.has_value())
            currentPlayer = newPlayer;

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

    string OnlineGame::processShipTypesRequest(ShipTypesRequest const &request) const
    {
        ShipTypes availableShipTypes = getShipTypes();
        ShipTypesResponse response(id, request.header.playerId, static_cast<uint8_t>(availableShipTypes.size()));
        string buf = serialize(response);

        for (auto const &[name, size] : availableShipTypes)
            buf += serialize(ShipType(name, static_cast<uint8_t>(size)));

        return buf;
    }

    string OnlineGame::processShipTypesRequest(string const &buf) const
    {
        return processShipTypesRequest(deserialize<ShipTypesRequest>(buf));
    }

    string OnlineGame::processMapRequest(MapRequest const &request) const
    {
        auto playerId = static_cast<unsigned long>(request.header.playerId);
        auto candidate = request.own ? getPlayer(playerId) : getOpponent(playerId);
        if (BOOST_UNLIKELY(!candidate.has_value()))
            throw InvalidRequest(request.own ? NO_SUCH_PLAYER : NO_OPPONENT);

        OnlinePlayer &player = candidate.value();
        Sea &sea = player.getSea();
        string map = sea.toString(request.own);
        MapResponse response(id, player.getId(),
                             static_cast<uint8_t>(sea.getWidth()),
                             static_cast<uint8_t>(sea.getHeight()),
                             static_cast<uint32_t>(map.size()));
        return serialize(response) + map;
    }

    string OnlineGame::processMapRequest(string const &buf) const
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

        ShipTypes availableShipTypes = getShipTypes();
        if (BOOST_UNLIKELY(availableShipTypes.count(type) == 0)) {
            result = PlacementResult::INVALID_SHIP_TYPE;
        } else if (BOOST_UNLIKELY(sea.hasShip(type))) {
            result = PlacementResult::ALREADY_PLACED;
        } else {
            Ship ship(type, Coordinate(request.x, request.y), availableShipTypes.at(type), request.orientation);
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

    StatusResponse OnlineGame::processStatusRequest(StatusRequest const &request) const
    {
        auto candidate = getPlayer(request.header.playerId);
        if (BOOST_UNLIKELY(!candidate.has_value()))
            throw InvalidRequest(NO_SUCH_PLAYER);

        OnlinePlayer &player = candidate.value();
        return StatusResponse(id, player.getId(), state);
    }

    string OnlineGame::processStatusRequest(string const &buf) const
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