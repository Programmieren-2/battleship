//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "boostwrap.h"
using boost::asio::ip::address;

#include "BasicShip.h"
using models::BasicShip;

#include "Coordinate.h"
using models::Coordinate;

#include "Game.h"
using models::Game;

#include "HitResult.h"
using models::HitResult;

#include "ShipTypes.h"
using models::ShipTypes;

#include "Client.h"
using tcp::Client;

#include "Messages.h"
#include "OnlinePlayer.h"
#include "ProtocolError.h"

#include "GameClient.h"

namespace multiplayer {
    GameClient::GameClient(address const &ipAddress, unsigned short port)
        : Client(ipAddress, port), gameId(0), playerId(0), gameOver(false), won(false)
    {}

    unsigned long GameClient::getGameId() const
    {
        return gameId;
    }

    unsigned long GameClient::getPlayerId() const
    {
        return playerId;
    }

    void GameClient::setGameId(unsigned long newGameId)
    {
        gameId = newGameId;
    }

    void GameClient::setPlayerId(unsigned long newPlayerId)
    {
        playerId = newPlayerId;
    }

    vector<ListedGame> GameClient::listGames() const
    {
        ListGamesRequest request;
        string buf = sendMessage(request);
        auto header = deserialize<ResponseHeader>(buf);
        if (header.type != ResponseType::LIST_GAMES_RESPONSE)
            throw ProtocolError(UNEXPECTED_RESPONSE_TYPE);

        return deserialize<vector<ListedGame>>(buf);
    }

    unsigned long GameClient::newGame(unsigned short width, unsigned short height)
    {
        NewGameRequest request(static_cast<uint8_t>(width), static_cast<uint8_t>(height));
        auto response = exchangeMessage<NewGameRequest, NewGameResponse>(request);
        return response.gameId;
    }

    unsigned long GameClient::newGame()
    {
        return newGame(Game<OnlinePlayer>::WIDTH, Game<OnlinePlayer>::HEIGHT);
    }

    bool GameClient::join(unsigned long newGameId, string const &playerName)
    {
        if (gameId != 0 || playerId != 0)
            throw ProtocolError(ALREADY_LOGGED_IN);

        LoginRequest request(newGameId, playerName);
        auto response = exchangeMessage<LoginRequest, LoginResponse>(request);

        if (response.accepted) {
            gameId = response.header.gameId;
            playerId = response.header.playerId;
        }

        return response.accepted;
    }

    bool GameClient::logout()
    {
        LogoutRequest request(gameId, playerId);
        auto response = exchangeMessage<LogoutRequest, LogoutResponse>(request);
        if (response.accepted) {
            playerId = 0;
            gameId = 0;
        }

        return response.accepted;
    }

    ShipTypes GameClient::getShipTypes() const
    {
        ShipTypesRequest request(gameId, playerId);
        string buf = sendMessage(request);
        return deserialize<ShipTypes>(buf);
    }

    string GameClient::getMap(bool own) const
    {
        MapRequest request(gameId, playerId, own);
        string buf = sendMessage(request);
        auto header = deserialize<ResponseHeader>(buf);

        if (BOOST_UNLIKELY(header.type != ResponseType::MAP_RESPONSE))
            throw ProtocolError(UNEXPECTED_RESPONSE_TYPE);

        return deserialize<string>(buf);
    }

    PlacementResult GameClient::placeShip(BasicShip const &ship)
    {
        ShipPlacementRequest request(gameId, playerId, ship);
        string buf = sendMessage(request);
        auto header = deserialize<ResponseHeader>(buf);

        if (BOOST_UNLIKELY(header.type != ResponseType::SHIP_PLACEMENT_RESPONSE))
            throw ProtocolError(UNEXPECTED_RESPONSE_TYPE);

        auto response = deserialize<ShipPlacementResponse>(buf);
        return response.result;
    }

    GameState GameClient::getStatus() const
    {
        StatusRequest request(gameId, playerId);
        auto response = exchangeMessage<StatusRequest, StatusResponse>(request);
        return response.state;
    }

    HitResult GameClient::fireAt(const Coordinate &target)
    {
        if (gameOver)
            throw ProtocolError(GAME_OVER);

        TurnRequest request(gameId, playerId, target);
        auto response = exchangeMessage<TurnRequest, TurnResponse>(request);
        won = response.won;
        return response.hitResult;
    }

    bool GameClient::isLoggedIn() const
    {
        return playerId != 0;
    }

    void GameClient::teardown()
    {
        if (isLoggedIn())
            logout();
    }
}