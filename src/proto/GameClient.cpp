//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;
using std::cout;

#include <optional>
using std::optional;

#include <stdexcept>
using std::invalid_argument;
using std::out_of_range;

#include <string>
using std::stoul;
using std::string;

#include <vector>
using std::vector;

#include <boost/config.hpp>

#include "BasicShip.h"
using models::BasicShip;

#include "Coordinate.h"
using models::Coordinate;

#include "Models.h"
using models::HitResult;
using models::PlacementResult;

#include "Sea.h"
using models::PlacementResult;

#include "Ship.h"
using models::ShipTypes;

#include "Client.h"
using net::Client;

#include "util.h"
using util::copyString;
using util::isExitCommand;
using util::readCommandLine;

#include "Messages.h"
#include "ProtocolError.h"

#include "GameClient.h"

namespace proto {
    GameClient::GameClient(string const &host, unsigned short port)
            : Client(host, port), gameId(0), playerId(0), won(false)
    {}

    GameClient::GameClient()
            : Client(), gameId(0), playerId(0)
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
            throw ProtocolError(header.type);

        auto response = deserialize<ListGamesResponse>(buf, true);
        vector<ListedGame> listedGames;
        ListedGame listedGame;
        size_t offset;

        for (unsigned long i = 0; i < response.games; i++) {
            offset = sizeof response + sizeof listedGame * i;
            listedGame = deserialize<ListedGame>(buf.substr(offset, sizeof listedGame));
            listedGames.push_back(listedGame);
        }

        return listedGames;
    }

    unsigned long GameClient::newGame(unsigned short width, unsigned short height)
    {
        NewGameRequest request(width, height);
        auto response = exchangeMessage<NewGameRequest, NewGameResponse>(request);
        return response.gameId;
    }

    bool GameClient::join(unsigned long gameId, string const &playerName)
    {
        LoginRequest request(gameId, playerName);
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
        return response.accepted;
    }

    ShipTypes GameClient::getShipTypes()
    {
        ShipTypesRequest request;
        request.header.playerId = playerId;
        string buf = sendMessage(request);
        auto response = deserialize<ShipTypesResponse>(buf, true);
        ShipTypes shipTypes;
        ShipType shipType;
        size_t offset;

        for (unsigned short i = 0; i < response.ships; i++) {
            offset = sizeof response + sizeof shipType * i;
            shipType = deserialize<ShipType>(buf.substr(offset, sizeof shipType), true);
            shipTypes[shipType.name] = shipType.size;
        }

        return shipTypes;
    }

    string GameClient::getMap(bool own)
    {
        MapRequest request;
        request.header.playerId = playerId;
        request.own = own;
        string buf = sendMessage(request);
        auto header = deserialize<ResponseHeader>(buf, true);

        if (BOOST_UNLIKELY(header.type != ResponseType::MAP_RESPONSE))
            throw ProtocolError(header.type);

        auto response = deserialize<MapResponse>(buf, true);
        return buf.substr(sizeof response, response.size);
    }

    PlacementResult GameClient::placeShip(BasicShip const &ship)
    {
        ShipPlacementRequest request;
        request.header.playerId = playerId;
        copyString(request.type, ship.getType(), sizeof request.type);
        request.x = ship.getAnchorPoint().getX();
        request.y = ship.getAnchorPoint().getY();
        request.orientation = ship.getOrientation();
        string buf = sendMessage(request);
        auto header = deserialize<ResponseHeader>(buf, true);

        if (BOOST_UNLIKELY(header.type != ResponseType::SHIP_PLACEMENT_RESPONSE))
            throw ProtocolError(header.type);

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
        TurnRequest request(gameId, playerId, target.getX(), target.getY());
        auto response = exchangeMessage<TurnRequest, TurnResponse>(request);
        won = response.won;
        return response.hitResult;
    }
}