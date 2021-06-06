//
// Created by rne on 15.05.21.
//

#include <algorithm>
using std::ranges::all_of;

#include <functional>
using std::reference_wrapper;

#include <optional>
using std::optional;

#include <string>
using std::string;

#include <stdexcept>
using std::out_of_range;

#include "exceptions.h"
using models::OutsideSeaBounds;
using models::Collision;

#include "Defaults.h"
#include "Coordinate.h"
using models::Coordinate;

#include "Game.h"
using models::Game;

#include "Sea.h"
using models::Sea;

#include "Ship.h"
using models::Ship;

#include "ShipTypes.h"
using models::ShipTypes;

#include "OnlinePlayer.h"

#include "util.h"
using util::copyString;

#include "Messages.h"
#include "OnlineGame.h"

namespace multiplayer {
    unsigned long OnlineGame::gameId = 0;

    OnlineGame::OnlineGame(unsigned short width, unsigned short height)
        : Game(width, height), id(++gameId), state(GameState::INITIAL)
    {}

    Sea OnlineGame::makeSea() const
    {
        return Sea(getWidth(), getHeight());
    }

    unsigned long OnlineGame::getId() const
    {
        return id;
    }

    GameState OnlineGame::getState() const
    {
        return state;
    }

    optional<reference_wrapper<OnlinePlayer const>> OnlineGame::getOpponent(unsigned long playerId) const
    {
        for (auto &player : getPlayers()) {
            if (player.get().getId() != playerId)
                return player;
        }

        return {};
    }

    optional<reference_wrapper<OnlinePlayer const>> OnlineGame::getPlayer(unsigned long playerId) const
    {
        for (auto &player : getPlayers()) {
            if (player.get().getId() == playerId)
                return player;
        }

        return {};
    }

    bool OnlineGame::allPlayersOnline() const
    {
        return getPlayers().size() == 2;
    }

    bool OnlineGame::allShipsPlaced(Sea const &sea) const
    {
        return all_of(getShipTypes(), [sea] (auto &pair) { return sea.hasShip(pair.first); });
    }

    bool OnlineGame::allPlayersReady() const
    {
        return all_of(getPlayers(), [this] (auto const &candidate) {
            return this->allShipsPlaced(candidate.get().getSea());
        });
    }

    PlacementResult OnlineGame::placeShip(ShipPlacementRequest const &request, Sea &sea) const
    {
        ShipTypes availableShipTypes = getShipTypes();
        if (BOOST_UNLIKELY(availableShipTypes.count(request.type) == 0))
            return INVALID_SHIP_TYPE;

        if (BOOST_UNLIKELY(sea.hasShip(request.type)))
            return ALREADY_PLACED;

        Ship ship(request.type,
                  Coordinate(request.x, request.y),
                  availableShipTypes.at(request.type),
                  request.orientation);

        try {
            sea.placeShip(ship);
        } catch (OutsideSeaBounds const &) {
            return OUTSIDE_SEA_BOUNDS;
        } catch (Collision const &) {
            return COLLISION;
        }

        return SHIP_PLACED;
    }

    LoginResponse OnlineGame::processLoginRequest(LoginRequest const &request) const
    {
        if (state == ABANDONED)
            return LoginResponse(0, 0, false);

        auto playerId = static_cast<uint32_t>(getPlayers().size() + 1);
        auto newPlayer = OnlinePlayer(playerId, request.playerName, makeSea());
        bool success = addPlayer(newPlayer);
        if (success && !currentPlayer)
            currentPlayer = newPlayer;

        return LoginResponse(id, success ? playerId : 0, success);
    }

    string OnlineGame::processLoginRequest(string const &buf) const
    {
        return serialize(processLoginRequest(deserialize<LoginRequest>(buf)));
    }

    LogoutResponse OnlineGame::processLogoutRequest(LogoutRequest const &request) const
    {
        auto player = getPlayer(request.header.playerId);
        if (BOOST_UNLIKELY(!player))
            return LogoutResponse(request.header.gameId, request.header.playerId, false);

        removePlayer(*player);
        state = isEmpty() ? ABANDONED : WAITING_FOR_PLAYERS;
        return LogoutResponse(request.header.gameId, request.header.playerId, true);
    }

    string OnlineGame::processLogoutRequest(string const &buf) const
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
        if (BOOST_UNLIKELY(!candidate))
            throw InvalidRequest(request.own ? NO_SUCH_PLAYER : NO_OPPONENT);

        OnlinePlayer const &player = *candidate;
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
        } catch (InvalidRequest const &invalidRequest) {
            return serialize(invalidRequest);
        }
    }

    ShipPlacementResponse OnlineGame::processShipPlacementRequest(ShipPlacementRequest const &request) const
    {
        auto candidate = getPlayer(request.header.playerId);
        if (BOOST_UNLIKELY(!candidate))
            throw InvalidRequest(NO_SUCH_PLAYER);

        OnlinePlayer const &player = *candidate;
        return ShipPlacementResponse(id, player.getId(), placeShip(request, player.getSea()));
    }

    string OnlineGame::processShipPlacementRequest(string const &buf) const
    {
        try {
            return serialize(processShipPlacementRequest(deserialize<ShipPlacementRequest>(buf)));
        } catch (InvalidRequest const &invalidRequest) {
            return serialize(invalidRequest);
        }
    }

    StatusResponse OnlineGame::processStatusRequest(StatusRequest const &request) const
    {
        auto candidate = getPlayer(request.header.playerId);
        if (BOOST_UNLIKELY(!candidate))
            throw InvalidRequest(NO_SUCH_PLAYER);

        OnlinePlayer const &player = *candidate;
        return StatusResponse(id, player.getId(), state);
    }

    string OnlineGame::processStatusRequest(string const &buf) const
    {
        try {
            return serialize(processStatusRequest(deserialize<StatusRequest>(buf)));
        } catch (InvalidRequest const &invalidRequest) {
            return serialize(invalidRequest);
        }
    }

    TurnResponse OnlineGame::processTurnRequest(TurnRequest const &request) const
    {
        auto playerRef = getPlayer(request.header.playerId);
        if (!playerRef)
            throw InvalidRequest(NO_SUCH_PLAYER);

        OnlinePlayer const &player = *playerRef;

        auto opponentRef = getOpponent(request.header.playerId);
        if (!opponentRef)
            throw InvalidRequest(NO_OPPONENT);

        OnlinePlayer const &opponent = *opponentRef;

        if (!currentPlayer || *currentPlayer != player)
            throw InvalidRequest(NOT_YOUR_TURN);

        Coordinate target = Coordinate(request.x, request.y);

        Sea &sea = opponent.getSea();
        if (!allShipsPlaced(sea))
            throw InvalidRequest(OPPONENT_NOT_READY);

        currentPlayer = opponent;
        return TurnResponse(id, player.getId(), sea.fireAt(target), sea.allShipsDestroyed());
    }

    string OnlineGame::processTurnRequest(string const &buf) const
    {
        try {
            return serialize(processTurnRequest(deserialize<TurnRequest>(buf)));
        } catch (InvalidRequest const &invalidRequest) {
            return serialize(invalidRequest);
        }
    }

    bool OnlineGame::operator==(const OnlineGame &other) const
    {
        return id == other.id;
    }

    bool OnlineGame::operator<(const OnlineGame &other) const
    {
        return id < other.id;
    }
}