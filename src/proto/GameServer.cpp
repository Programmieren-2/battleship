//
// Created by rne on 08.05.21.
//

#include <algorithm>
using std::find_if;

#include <functional>
using std::reference_wrapper;

#include <optional>
using std::optional;

#include <string>
using std::string;

#include "boostwrap.h"
using boost::asio::ip::address;

#include "Server.h"
using net::Server;

#include "Messages.h"
#include "OnlineGame.h"

#include "GameServer.h"

namespace proto {
    unsigned long GameServer::gameId = 0;

    GameServer::GameServer(address const &ipAddress, unsigned short port)
        : Server(ipAddress, port)
    {}

    optional<reference_wrapper<OnlineGame>> GameServer::getGame(unsigned long id)
    {
        for (auto &game : games) {
            if (game.getId() == id)
                return game;
        }

        return {};
    }

    unsigned long GameServer::addGame(unsigned short width, unsigned short height)
    {
        games.emplace_back(++gameId, width, height);
        return gameId;
    }

    bool GameServer::removeGame(unsigned long id)
    {
        auto pos = find_if(games.begin(), games.end(), [id](OnlineGame const &game) {
            return game.getId() == id;
        });
        if (pos == games.end())
            return false;

        games.erase(pos);
        return true;
    }

    bool GameServer::removeGame(OnlineGame const &game)
    {
        return removeGame(game.getId());
    }

    void GameServer::cleanup()
    {
        for (auto const &game : games) {
            if (game.getState() == ABANDONED)
                removeGame(game);
        }
    }

    string GameServer::processListGamesRequest() const
    {
        ListGamesResponse response(static_cast<uint32_t>(games.size()));
        string buf = serialize(response);

        for (auto const &game : games) {
            ListedGame listedGame(game.getId(),
                                  static_cast<uint8_t>(game.getWidth()),
                                  static_cast<uint8_t>(game.getHeight()),
                                  static_cast<uint8_t>(game.getPlayerCount()));
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

    string GameServer::handleRequest(string const &buf)
    {
        auto header = deserialize<RequestHeader>(buf, true);

        switch (header.type) {
            case NOOP:
                return serialize(InvalidRequest());
            case LIST_GAMES_REQUEST:
                return processListGamesRequest();
            case NEW_GAME_REQUEST:
                return processNewGameRequest(buf);
            default:
                break;
        }

        auto candidate = getGame(header.gameId);
        if (BOOST_UNLIKELY(!candidate))
            return serialize(InvalidRequest(ErrorType::NO_SUCH_GAME));

        auto &game = candidate.value().get();

        switch (header.type) {
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
            default:
                return serialize(InvalidRequest(ErrorType::UNKNOWN));
        }
    }
}
