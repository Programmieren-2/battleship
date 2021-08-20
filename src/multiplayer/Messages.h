//
// Created by rne on 08.05.21.
//

#pragma once

#include <cstring>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "boostwrap.h"

#include "BasicShip.h"
#include "Coordinate.h"
#include "Sea.h"
#include "ShipTypes.h"

#include "util.h"

namespace multiplayer {
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

    enum PlacementResult : unsigned char {
        SHIP_PLACED,
        OUTSIDE_SEA_BOUNDS,
        COLLISION,
        ALREADY_PLACED,
        INVALID_SHIP_TYPE,
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

        RequestHeader(RequestType type, uint32_t gameId, uint32_t playerId);
        RequestHeader();
    };

    struct ResponseHeader {
        ResponseType type;
        uint32_t gameId;
        uint32_t playerId;

        ResponseHeader(ResponseType type, uint32_t gameId, uint32_t playerId);
        ResponseHeader();
    };

    struct Request {
        RequestHeader header;

        Request(RequestType type, uint32_t gameId, uint32_t playerId);
    };

    struct Response {
        ResponseHeader header;

        Response(ResponseType type, uint32_t gameId, uint32_t playerId);
    };

    struct ListGamesRequest : Request {
        ListGamesRequest();
    };

    struct ListGamesResponse : Response {
        uint32_t games;

        explicit ListGamesResponse(uint32_t games);
        ListGamesResponse();
    };

    struct ListedGame {
        uint32_t id;
        uint8_t width;
        uint8_t height;
        uint8_t players;

        ListedGame(uint32_t id, uint8_t width, uint8_t height, uint8_t players);
        ListedGame();

        friend std::ostream& operator<<(std::ostream &out, ListedGame const &game);
    };

    struct NewGameRequest : Request {
        uint8_t width;
        uint8_t height;

        NewGameRequest(uint8_t width, uint8_t height);
        NewGameRequest();
    };

    struct NewGameResponse : Response {
        uint32_t gameId;

        NewGameResponse(uint32_t playerId, uint32_t gameId);
        NewGameResponse();
    };
    
    struct LoginRequest : Request {
        char playerName[32];

        LoginRequest(uint32_t gameId, std::string const &name);
        LoginRequest();
    };

    struct LoginResponse : Response {
        bool accepted;

        LoginResponse(uint32_t gameId, uint32_t playerId, bool accepted);
        LoginResponse();
    };

    struct LogoutRequest : Request {
        LogoutRequest(uint32_t gameId, uint32_t playerId);
        LogoutRequest();
    };

    struct LogoutResponse : Response {
        bool accepted;

        LogoutResponse(uint32_t gameId, uint32_t playerId, bool accepted);
        LogoutResponse();
    };

    struct ShipTypesRequest : Request {
        ShipTypesRequest(uint32_t gameId, uint32_t playerId);
        ShipTypesRequest();
    };

    struct ShipTypesResponse : Response {
        uint8_t ships;

        ShipTypesResponse(uint32_t gameId, uint32_t playerId, uint8_t ships);
        ShipTypesResponse();
    };

    /*
     * ShipType is a special appendix packet to ShipTypesResponse and thus does not need a header.
     */
    struct ShipType {
        char name[32];
        uint8_t size;

        ShipType(std::string const &type, uint8_t size);
        ShipType();
    };

    struct MapRequest : Request {
        bool own;

        MapRequest(uint32_t gameId, uint32_t playerId, bool own);
        MapRequest();
    };

    struct MapResponse : Response {
        uint8_t width;
        uint8_t height;
        uint32_t size;  // Followed by so many ASCII chars for ASCII representation of the map.

        MapResponse(uint32_t gameId, uint32_t playerId, uint8_t width, uint8_t height, uint32_t size);
        MapResponse();
    };

    struct ShipPlacementRequest : Request {
        char type[32];
        uint8_t x;
        uint8_t y;
        models::Orientation orientation;

        ShipPlacementRequest(uint32_t gameId, uint32_t playerId, std::string const &typeName, uint8_t x, uint8_t y,
                             models::Orientation const &orientation);
        ShipPlacementRequest(uint32_t gameId, uint32_t playerId, std::string const &typeName,
                             models::Coordinate const &coordinate, models::Orientation const &orientation);
        ShipPlacementRequest(uint32_t gameId, uint32_t playerId, models::BasicShip const &ship);
        ShipPlacementRequest();
    };

    struct ShipPlacementResponse : Response {
        PlacementResult result;

        ShipPlacementResponse(uint32_t gameId, uint32_t playerId, PlacementResult result);
        ShipPlacementResponse();
    };

    struct StatusRequest : Request {
        StatusRequest(uint32_t gameId, uint32_t playerId);
        StatusRequest();
    };

    struct StatusResponse : Response {
        GameState state;

        StatusResponse(uint32_t gameId, uint32_t playerId, GameState state);
        StatusResponse();
    };

    struct TurnRequest : Request {
        uint8_t x;
        uint8_t y;

        TurnRequest(uint32_t gameId, uint32_t playerId, uint8_t x, uint8_t y);
        TurnRequest(uint32_t gameId, uint32_t playerId, models::Coordinate const &coordinate);
        TurnRequest();
    };

    struct TurnResponse : Response {
        models::HitResult hitResult;
        bool won;

        TurnResponse(uint32_t gameId, uint32_t playerId, models::HitResult hitResult, bool won);
        TurnResponse();
    };

    struct InvalidRequest : Response {
        ErrorType error;

        InvalidRequest(uint32_t gameId, uint32_t playerId, ErrorType error);
        InvalidRequest(uint32_t playerId, ErrorType error);
        explicit InvalidRequest(ErrorType error);
        InvalidRequest();
    };

#pragma pack(pop)

    template <typename Message>
    Message deserialize(std::string const &buf);

    template <>
    RequestHeader deserialize(std::string const &buf);

    template <>
    ResponseHeader deserialize(std::string const &buf);

    template <>
    ListGamesResponse deserialize(std::string const &buf);

    template <>
    std::vector<ListedGame> deserialize(std::string const &buf);

    template <>
    ShipTypesResponse deserialize(std::string const &buf);

    template <>
    models::ShipTypes deserialize(std::string const &buf);

    template <>
    MapResponse deserialize(std::string const &buf);

    template <>
    std::string deserialize(std::string const &buf);

    template <typename Message>
    std::string serialize(Message const &msg);

#include "Messages.inl"
}
