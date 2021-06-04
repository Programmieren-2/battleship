//
// Created by rne on 08.05.21.
//

#pragma once

#include <cstring>
#include <exception>
#include <iostream>
#include <string>

#include "boostwrap.h"

#include "BasicShip.h"
#include "Coordinate.h"
#include "Models.h"
#include "Sea.h"

#include "util.h"

namespace proto {
    enum GameState : unsigned char {
        INITIAL,
        WAITING_FOR_PLAYERS,
        WAITING_FOR_YOUR_SHIP_PLACEMENTS,
        WAITING_FOR_OPPONENTS_SHIP_PLACEMENTS,
        WAITING_FOR_YOUR_TURN,
        WAITING_FOR_OPPONENTS_TURN,
        ABANDONED,
    };

    enum RequestType : unsigned char {
        NOOP,
        LIST_GAMES_REQUEST,
        NEW_GAME_REQUEST,
        LOGIN_REQUEST,
        LOGOUT_REQUEST,
        SHIP_TYPES_REQUEST,
        MAP_REQUEST,
        SHIP_PLACEMENT_REQUEST,
        STATUS_REQUEST,
        TURN_REQUEST,
    };

    enum ResponseType : unsigned char {
        INVALID_REQUEST,
        LIST_GAMES_RESPONSE,
        NEW_GAME_RESPONSE,
        LOGIN_RESPONSE,
        LOGOUT_RESPONSE,
        SHIP_TYPES_RESPONSE,
        MAP_RESPONSE,
        SHIP_PLACEMENT_RESPONSE,
        STATUS_RESPONSE,
        TURN_RESPONSE,
    };

    enum ErrorType : unsigned char {
        UNKNOWN,
        NO_SUCH_GAME,
        NO_SUCH_PLAYER,
        NO_OPPONENT,
        NOT_YOUR_TURN,
        OPPONENT_NOT_READY,
        UNEXPECTED_RESPONSE_TYPE,
        ALREADY_LOGGED_IN,
        GAME_OVER,
    };

#pragma pack(push, 1)

    struct RequestHeader {
        RequestType type;
        uint32_t gameId;
        uint32_t playerId;

        RequestHeader(RequestType type, uint32_t gameId, uint32_t playerId)
            : type(type), gameId(gameId), playerId(playerId)
        {}

        RequestHeader()
            : RequestHeader(NOOP, 0, 0)
        {}
    };

    struct ResponseHeader {
        ResponseType type;
        uint32_t gameId;
        uint32_t playerId;

        ResponseHeader(ResponseType type, uint32_t gameId, uint32_t playerId)
            : type(type), gameId(gameId), playerId(playerId)
        {}

        ResponseHeader()
            : ResponseHeader(INVALID_REQUEST, 0, 0)
        {}
    };

    struct Request {
        RequestHeader header;

        Request(RequestType type, uint32_t gameId, uint32_t playerId)
            : header(RequestHeader(type, gameId, playerId))
        {}
    };

    struct Response {
        ResponseHeader header;

        Response(ResponseType type, uint32_t gameId, uint32_t playerId)
            : header(ResponseHeader(type, gameId, playerId))
        {}
    };

    struct ListGamesRequest : Request {
        ListGamesRequest()
            : Request(LIST_GAMES_REQUEST, 0, 0)
        {}
    };

    struct ListGamesResponse : Response {
        uint32_t games;

        ListGamesResponse(uint32_t games)
            : Response(LIST_GAMES_RESPONSE, 0, 0), games(games)
        {}

        ListGamesResponse()
            : ListGamesResponse(0)
        {}
    };

    struct ListedGame {
        uint32_t id;
        uint8_t width;
        uint8_t height;
        uint8_t players;

        ListedGame(uint32_t id, uint8_t width, uint8_t height, uint8_t players)
            : id(id), width(width), height(height), players(players)
        {}

        ListedGame()
            : ListedGame(0, 0, 0, 0)
        {}
    };

    struct NewGameRequest : Request {
        uint8_t width;
        uint8_t height;

        NewGameRequest(uint8_t width, uint8_t height)
            : Request(NEW_GAME_REQUEST, 0 ,0), width(width), height(height)
        {}

        NewGameRequest()
            : NewGameRequest(0, 0)
        {}
    };

    struct NewGameResponse : Response {
        uint32_t gameId;

        NewGameResponse(uint32_t playerId, uint32_t gameId)
            : Response(NEW_GAME_RESPONSE, gameId, playerId), gameId(gameId)
        {}

        NewGameResponse()
            : NewGameResponse(0 ,0)
        {}
    };
    
    struct LoginRequest : Request {
        char playerName[32];

        LoginRequest(uint32_t gameId, std::string const &name)
            : Request(LOGIN_REQUEST, gameId, 0), playerName("")
        {
            if (!name.empty())
                util::copyString(playerName, name, sizeof playerName);
        }

        LoginRequest()
            : LoginRequest(0, "")
        {}
    };

    struct LoginResponse : Response {
        bool accepted;

        LoginResponse(uint32_t gameId, uint32_t playerId, bool accepted)
            : Response(LOGIN_RESPONSE, gameId, playerId), accepted(accepted)
        {}

        LoginResponse()
            : LoginResponse(0, 0, false)
        {}
    };

    struct LogoutRequest : Request {
        LogoutRequest(uint32_t gameId, uint32_t playerId)
            : Request(LOGOUT_REQUEST, gameId, playerId)
        {}

        LogoutRequest()
            : LogoutRequest(0, 0)
        {}
    };

    struct LogoutResponse : Response {
        bool accepted;

        LogoutResponse(uint32_t gameId, uint32_t playerId, bool accepted)
            : Response(LOGOUT_RESPONSE, gameId, playerId), accepted(accepted)
        {}

        LogoutResponse()
            : LogoutResponse(0, 0, false)
        {}
    };

    struct ShipTypesRequest : Request {
        ShipTypesRequest(uint32_t gameId, uint32_t playerId)
            : Request(SHIP_TYPES_REQUEST, gameId, playerId)
        {}

        ShipTypesRequest()
            : ShipTypesRequest(0, 0)
        {}
    };

    struct ShipTypesResponse : Response {
        uint8_t ships;

        ShipTypesResponse(uint32_t gameId, uint32_t playerId, uint8_t ships)
            : Response(SHIP_TYPES_RESPONSE, gameId, playerId), ships(ships)
        {}

        ShipTypesResponse()
            : ShipTypesResponse(0, 0, 0)
        {}
    };

    /*
     * ShipType is a special appendix packet to ShipTypesResponse and thus does not need a header.
     */
    struct ShipType {
        char name[32];
        uint8_t size;

        ShipType(string const &type, uint8_t size)
            : name(""), size(size)
        {
            if (!type.empty())
                util::copyString(name, type, sizeof name);
        }

        ShipType()
            : ShipType("", 0)
        {}
    };

    struct MapRequest : Request {
        bool own;

        MapRequest(uint32_t gameId, uint32_t playerId, bool own)
            : Request(MAP_REQUEST, gameId, playerId), own(own)
        {}

        MapRequest()
            : MapRequest(0, 0, false)
        {}
    };

    struct MapResponse : Response {
        uint8_t width;
        uint8_t height;
        uint32_t size;  // Followed by so many ASCII chars for ASCII representation of the map.

        MapResponse(uint32_t gameId, uint32_t playerId, uint8_t width, uint8_t height, uint32_t size)
            : Response(MAP_RESPONSE, gameId, playerId), width(width), height(height), size(size)
        {}

        MapResponse()
            : MapResponse(0, 0, 0, 0, 0)
        {}
    };

    struct ShipPlacementRequest : Request {
        char type[32];
        uint8_t x;
        uint8_t y;
        models::Orientation orientation;

        ShipPlacementRequest(uint32_t gameId, uint32_t playerId, string const &typeName, uint8_t x, uint8_t y,
                             models::Orientation orientation)
             : Request(SHIP_PLACEMENT_REQUEST, gameId, playerId), type(""), x(x), y(y), orientation(orientation)
        {
            if (!typeName.empty())
                util::copyString(type, typeName, sizeof type);
        }

        ShipPlacementRequest(uint32_t gameId, uint32_t playerId, string const &typeName,
                             models::Coordinate const &coordinate, models::Orientation orientation)
            : ShipPlacementRequest(gameId, playerId, typeName, static_cast<uint8_t>(coordinate.first),
                                   static_cast<uint8_t>(coordinate.second), orientation)
        {}

        ShipPlacementRequest(uint32_t gameId, uint32_t playerId, models::BasicShip const &ship)
            : ShipPlacementRequest(gameId, playerId, ship.getType(), ship.getAnchorPoint(),
                                   ship.getOrientation())
        {}

        ShipPlacementRequest()
            : ShipPlacementRequest(0, 0, "", 0, 0, models::Orientation::X)
        {}
    };

    struct ShipPlacementResponse : Response {
        models::PlacementResult result;

        ShipPlacementResponse(uint32_t gameId, uint32_t playerId, models::PlacementResult result)
            : Response(SHIP_PLACEMENT_RESPONSE, gameId, playerId), result(result)
        {}

        ShipPlacementResponse()
            : ShipPlacementResponse(0, 0, models::PlacementResult::INVALID_SHIP_TYPE)
        {}
    };

    struct StatusRequest : Request {
        StatusRequest(uint32_t gameId, uint32_t playerId)
            : Request(STATUS_REQUEST, gameId, playerId)
        {}

        StatusRequest()
            : StatusRequest(0, 0)
        {}
    };

    struct StatusResponse : Response {
        GameState state;

        StatusResponse(uint32_t gameId, uint32_t playerId, GameState state)
            : Response(STATUS_RESPONSE, gameId, playerId), state(state)
        {}

        StatusResponse()
            : StatusResponse(0, 0, GameState::INITIAL)
        {}
    };

    struct TurnRequest : Request {
        uint8_t x;
        uint8_t y;

        TurnRequest(uint32_t gameId, uint32_t playerId, uint8_t x, uint8_t y)
            : Request(TURN_REQUEST, gameId, playerId), x(x), y(y)
        {}

        TurnRequest(uint32_t gameId, uint32_t playerId, models::Coordinate const &coordinate)
            : TurnRequest(gameId, playerId, static_cast<uint8_t>(coordinate.first),
                          static_cast<uint8_t>(coordinate.second))
        {}

        TurnRequest()
            : TurnRequest(0, 0, 0, 0)
        {}
    };

    struct TurnResponse : Response {
        models::HitResult hitResult;
        bool won;

        TurnResponse(uint32_t gameId, uint32_t playerId, models::HitResult hitResult, bool won)
            : Response(TURN_RESPONSE, gameId, playerId), hitResult(hitResult), won(won)
        {}

        TurnResponse()
            : TurnResponse(0, 0, models::HitResult::MISSED, false)
        {}
    };

    struct InvalidRequest : Response {
        ErrorType error;

        InvalidRequest(uint32_t gameId, uint32_t playerId, ErrorType error)
            : Response(INVALID_REQUEST, gameId, playerId), error(error)
        {}

        InvalidRequest(uint32_t playerId, ErrorType error)
            : InvalidRequest(0, playerId, error)
        {}

        explicit InvalidRequest(ErrorType error)
            : InvalidRequest(0, error)
        {}

        InvalidRequest()
            : InvalidRequest(ErrorType::UNKNOWN)
        {}
    };

#pragma pack(pop)

    /*
     * Convert a byte string into a message struct.
     */
    template <typename Message>
    Message deserialize(std::string const &buf, bool partialProcessing = false)
    {
        Message msg;
        size_t bufSize = buf.length();
        size_t msgSize = sizeof msg;

        if (msgSize > bufSize)
            throw std::underflow_error("Message size exceeds buffer size.");

        if (msgSize < bufSize && !partialProcessing)
            throw std::overflow_error("Buffer size exceeds message size.");

        std::memcpy(&msg, &buf[0], (msgSize < bufSize) ? msgSize : bufSize);
        return msg;
    }

    /*
     * Convert a message struct into a byte string.
     */
    template <typename Message>
    std::string serialize(Message const &msg)
    {
        auto ptr = reinterpret_cast<const char*>(&msg);
        return std::string(ptr, ptr + sizeof msg);
    }
}