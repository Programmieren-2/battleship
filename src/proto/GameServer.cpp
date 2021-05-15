//
// Created by rne on 08.05.21.
//

#include <optional>
using std::optional;

#include <string>
using std::string;

#include <boost/config.hpp>

#include "Net.h"
#include "Server.h"
using net::Server;

#include "Messages.h"
#include "OnlineGame.h"

#include "GameServer.h"

namespace proto {
    unsigned long GameServer::gameId = 0;

    GameServer::GameServer(const string &host, unsigned short port)
        : Server(host, port), games(Games())
    {}

    GameServer::GameServer()
        : GameServer(net::Defaults::HOST, net::Defaults::PORT)
    {}

    optional<OnlineGame> GameServer::getGame(unsigned long id)
    {
        optional<OnlineGame> result;

        for (auto &game : games) {
            if (game.getId() == id)
                return result = game;
        }

        return result;
    }

    unsigned long GameServer::addGame(unsigned short width, unsigned short height)
    {
        games.push_back(OnlineGame(gameId++, width, height));
        return gameId;
    }

    string GameServer::processNewGameRequest(NewGameRequest const &request)
    {
        NewGameResponse response;
        response.gameId = addGame(request.width, request.height);
        return serialize(response);
    }

    string GameServer::handleRequest(const string &buf) {
        auto header = deserialize<RequestHeader>(buf, true);
        unsigned long gameId = header.gameId;
        auto candidate = getGame(gameId);

        if (BOOST_UNLIKELY(!candidate.has_value()))
            return serialize(InvalidRequest());

        auto &game = candidate.value();

        switch (header.type) {
            case NOOP:
                return serialize(InvalidRequest());
            case NEW_GAME_REQUEST:
                return processNewGameRequest(deserialize<NewGameRequest>(buf));
            case LOGIN_REQUEST:
                return game.processLoginRequest(deserialize<LoginRequest>(buf));
            case SHIP_TYPES_REQUEST:
                return game.processShipTypesRequest(deserialize<ShipTypesRequest>(buf));
            case MAP_REQUEST:
                return game.processMapRequest(deserialize<MapRequest>(buf));
            case SHIP_PLACEMENT_REQUEST:
                return game.processShipPlacementRequest(deserialize<ShipPlacementRequest>(buf));
            case STATUS_REQUEST:
                return game.processStatusRequest(deserialize<StatusRequest>(buf));
            case TURN_REQUEST:
                return game.processTurnRequest(deserialize<TurnRequest>(buf));
        }
    }
}