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

    string GameServer::processListGamesRequest()
    {
        ListGamesResponse response(games.size());
        string buf = serialize(response);

        for (auto const &game : games) {
            ListedGame listedGame(game.getId(), game.getWidth(), game.getHeight(), game.getPlayerCount());
            buf += serialize(listedGame);
        }

        return buf;
    }

    NewGameResponse GameServer::processNewGameRequest(NewGameRequest const &request)
    {
        auto newGameId = addGame(request.width, request.height);
        return NewGameResponse(request.header.playerId, newGameId);
    }

    string GameServer::processNewGameRequest(string const &buf)
    {
        return serialize(processNewGameRequest(deserialize<NewGameRequest>(buf)));
    }

    string GameServer::handleRequest(OnlineGame &game, RequestType type, string const &buf)
    {
        switch (type) {
            case NOOP:
                return serialize(InvalidRequest());
            case LIST_GAMES_REQUEST:
                return processListGamesRequest();
            case NEW_GAME_REQUEST:
                return processNewGameRequest(buf);
            case LOGIN_REQUEST:
                return game.processLoginRequest(buf);
            case LOGOUT_REQUEST:
                return game.processLogoutRequest(buf);
            case SHIP_TYPES_REQUEST:
                return game.processShipTypesRequest(buf);
            case MAP_REQUEST:
                return game.processMapRequest(buf);
            case SHIP_PLACEMENT_REQUEST:
                return game.processShipPlacementRequest(buf);
            case STATUS_REQUEST:
                return game.processStatusRequest(buf);
            case TURN_REQUEST:
                return game.processTurnRequest(buf);
        }
    }

    string GameServer::handleRequest(string const &buf) {
        auto header = deserialize<RequestHeader>(buf, true);
        auto candidate = getGame(header.gameId);
        if (BOOST_UNLIKELY(!candidate.has_value()))
            return serialize(InvalidRequest(ErrorType::NO_SUCH_GAME));

        return handleRequest(candidate.value(), header.type, buf);
    }
}
