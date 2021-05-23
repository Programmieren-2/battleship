//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "boostwrap.h"

#include "BasicShip.h"
using models::BasicShip;

#include "Coordinate.h"
using models::Coordinate;

#include "Models.h"
using models::HitResult;
using models::PlacementResult;
using models::ShipTypes;

#include "Client.h"
using net::Client;

#include "Net.h"
using net::IPAddress;

#include "Messages.h"
#include "ProtocolError.h"

#include "GameClient.h"

namespace proto {
    GameClient::GameClient(IPAddress ipAddress, unsigned short port)
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

    vector<ListedGame> GameClient::listGames()
    {
        ListGamesRequest request;
        string buf = sendMessage(request);
        auto header = deserialize<ResponseHeader>(buf, true);
        if (header.type != ResponseType::LIST_GAMES_RESPONSE)
            throw ProtocolError(UNEXPECTED_RESPONSE_TYPE);

        auto response = deserialize<ListGamesResponse>(buf, true);
        vector<ListedGame> listedGames;
        ListedGame listedGame;
        size_t offset;

        for (unsigned long i = 0; i < response.games; ++i) {
            offset = sizeof response + sizeof listedGame * i;
            listedGame = deserialize<ListedGame>(buf.substr(offset, sizeof listedGame));
            listedGames.push_back(listedGame);
        }

        return listedGames;
    }

    unsigned long GameClient::newGame(unsigned short width, unsigned short height)
    {
        NewGameRequest request(static_cast<uint8_t>(width), static_cast<uint8_t>(height));
        auto response = exchangeMessage<NewGameRequest, NewGameResponse>(request);
        return response.gameId;
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

    ShipTypes GameClient::getShipTypes()
    {
        ShipTypesRequest request(gameId, playerId);
        string buf = sendMessage(request);
        auto response = deserialize<ShipTypesResponse>(buf, true);
        ShipTypes shipTypes;
        ShipType shipType;
        size_t offset;

        for (unsigned short i = 0; i < response.ships; ++i) {
            offset = sizeof response + sizeof shipType * i;
            shipType = deserialize<ShipType>(buf.substr(offset, sizeof shipType), true);
            shipTypes[shipType.name] = shipType.size;
        }

        return shipTypes;
    }

    string GameClient::getMap(bool own)
    {
        MapRequest request(gameId, playerId, own);
        string buf = sendMessage(request);
        auto header = deserialize<ResponseHeader>(buf, true);

        if (BOOST_UNLIKELY(header.type != ResponseType::MAP_RESPONSE))
            throw ProtocolError(UNEXPECTED_RESPONSE_TYPE);

        auto response = deserialize<MapResponse>(buf, true);
        return buf.substr(sizeof response, response.size);
    }

    PlacementResult GameClient::placeShip(BasicShip const &ship)
    {
        ShipPlacementRequest request(gameId, playerId, ship);
        string buf = sendMessage(request);
        auto header = deserialize<ResponseHeader>(buf, true);

        if (BOOST_UNLIKELY(header.type != ResponseType::SHIP_PLACEMENT_RESPONSE))
            throw ProtocolError(UNEXPECTED_RESPONSE_TYPE);

        auto response = deserialize<ShipPlacementResponse>(buf);
        return response.result;
    }

    GameState GameClient::getStatus()
    {
        StatusRequest request(gameId, playerId);
        auto response = exchangeMessage<StatusRequest, StatusResponse>(request);
        return response.state;
    }

    models::HitResult GameClient::fireAt(const Coordinate &target)
    {
        if (gameOver)
            throw ProtocolError(GAME_OVER);

        TurnRequest request(gameId, playerId, static_cast<uint8_t>(target.getX()), static_cast<uint8_t>(target.getY()));
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