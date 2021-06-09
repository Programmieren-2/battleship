//
// Created by rne on 09.06.21.
//

#include <iostream>
using std::ostream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Messages.h"

namespace multiplayer {
    RequestHeader::RequestHeader(RequestType type, uint32_t gameId, uint32_t playerId)
        : type(type), gameId(gameId), playerId(playerId)
    {}

    RequestHeader::RequestHeader()
        : RequestHeader(NOOP, 0, 0)
    {}

    ResponseHeader::ResponseHeader(ResponseType type, uint32_t gameId, uint32_t playerId)
        : type(type), gameId(gameId), playerId(playerId)
    {}

    ResponseHeader::ResponseHeader()
        : ResponseHeader(INVALID_REQUEST, 0, 0)
    {}

    Request::Request(RequestType type, uint32_t gameId, uint32_t playerId)
        : header(RequestHeader(type, gameId, playerId))
    {}

    Response::Response(ResponseType type, uint32_t gameId, uint32_t playerId)
        : header(ResponseHeader(type, gameId, playerId))
    {}

    ListGamesRequest::ListGamesRequest()
        : Request(LIST_GAMES_REQUEST, 0, 0)
    {}

    ListGamesResponse::ListGamesResponse(uint32_t games)
        : Response(LIST_GAMES_RESPONSE, 0, 0), games(games)
    {}

    ListGamesResponse::ListGamesResponse()
        : ListGamesResponse(0)
    {}

    ListedGame::ListedGame(uint32_t id, uint8_t width, uint8_t height, uint8_t players)
        : id(id), width(width), height(height), players(players)
    {}

    ListedGame::ListedGame()
        : ListedGame(0, 0, 0, 0)
    {}

    NewGameRequest::NewGameRequest(uint8_t width, uint8_t height)
        : Request(NEW_GAME_REQUEST, 0 ,0), width(width), height(height)
    {}

    NewGameRequest::NewGameRequest()
        : NewGameRequest(0, 0)
    {}

    NewGameResponse::NewGameResponse(uint32_t playerId, uint32_t gameId)
        : Response(NEW_GAME_RESPONSE, gameId, playerId), gameId(gameId)
    {}

    NewGameResponse::NewGameResponse()
        : NewGameResponse(0 ,0)
    {}

    LoginRequest::LoginRequest(uint32_t gameId, string const &name)
        : Request(LOGIN_REQUEST, gameId, 0), playerName("")
    {
        if (!name.empty())
            util::copyString(playerName, name, sizeof playerName);
    }

    LoginRequest::LoginRequest()
        : LoginRequest(0, "")
    {}

    LoginResponse::LoginResponse(uint32_t gameId, uint32_t playerId, bool accepted)
        : Response(LOGIN_RESPONSE, gameId, playerId), accepted(accepted)
    {}

    LoginResponse::LoginResponse()
        : LoginResponse(0, 0, false)
    {}

    LogoutRequest::LogoutRequest(uint32_t gameId, uint32_t playerId)
        : Request(LOGOUT_REQUEST, gameId, playerId)
    {}

    LogoutRequest::LogoutRequest()
        : LogoutRequest(0, 0)
    {}

    LogoutResponse::LogoutResponse(uint32_t gameId, uint32_t playerId, bool accepted)
        : Response(LOGOUT_RESPONSE, gameId, playerId), accepted(accepted)
    {}

    LogoutResponse::LogoutResponse()
        : LogoutResponse(0, 0, false)
    {}

    ShipTypesRequest::ShipTypesRequest(uint32_t gameId, uint32_t playerId)
        : Request(SHIP_TYPES_REQUEST, gameId, playerId)
    {}

    ShipTypesRequest::ShipTypesRequest()
        : ShipTypesRequest(0, 0)
    {}

    ShipTypesResponse::ShipTypesResponse(uint32_t gameId, uint32_t playerId, uint8_t ships)
        : Response(SHIP_TYPES_RESPONSE, gameId, playerId), ships(ships)
    {}

    ShipTypesResponse::ShipTypesResponse()
        : ShipTypesResponse(0, 0, 0)
    {}

    ShipType::ShipType(string const &type, uint8_t size)
        : name(""), size(size)
    {
        if (!type.empty())
            util::copyString(name, type, sizeof name);
    }

    ShipType::ShipType()
        : ShipType("", 0)
    {}

    MapRequest::MapRequest(uint32_t gameId, uint32_t playerId, bool own)
        : Request(MAP_REQUEST, gameId, playerId), own(own)
    {}

    MapRequest::MapRequest()
        : MapRequest(0, 0, false)
    {}

    MapResponse::MapResponse(uint32_t gameId, uint32_t playerId, uint8_t width, uint8_t height, uint32_t size)
        : Response(MAP_RESPONSE, gameId, playerId), width(width), height(height), size(size)
    {}

    MapResponse::MapResponse()
        : MapResponse(0, 0, 0, 0, 0)
    {}

    ShipPlacementRequest::ShipPlacementRequest(uint32_t gameId, uint32_t playerId, string const &typeName,
                                               uint8_t x, uint8_t y, models::Orientation const &orientation)
        : Request(SHIP_PLACEMENT_REQUEST, gameId, playerId), type(""), x(x), y(y), orientation(orientation)
    {
        if (!typeName.empty())
            util::copyString(type, typeName, sizeof type);
    }

    ShipPlacementRequest::ShipPlacementRequest(uint32_t gameId, uint32_t playerId, string const &typeName,
                                               models::Coordinate const &coordinate, models::Orientation const &orientation)
        : ShipPlacementRequest(gameId, playerId, typeName, static_cast<uint8_t>(coordinate.first),
                               static_cast<uint8_t>(coordinate.second), orientation)
    {}

    ShipPlacementRequest::ShipPlacementRequest(uint32_t gameId, uint32_t playerId, models::BasicShip const &ship)
        : ShipPlacementRequest(gameId, playerId, ship.getType(), ship.getAnchorPoint(), ship.getOrientation())
    {}

    ShipPlacementRequest::ShipPlacementRequest()
        : ShipPlacementRequest(0, 0, "", 0, 0, models::Orientation::X)
    {}

    ShipPlacementResponse::ShipPlacementResponse(uint32_t gameId, uint32_t playerId, PlacementResult result)
        : Response(SHIP_PLACEMENT_RESPONSE, gameId, playerId), result(result)
    {}

    ShipPlacementResponse::ShipPlacementResponse()
        : ShipPlacementResponse(0, 0, INVALID_SHIP_TYPE)
    {}

    StatusRequest::StatusRequest(uint32_t gameId, uint32_t playerId)
        : Request(STATUS_REQUEST, gameId, playerId)
    {}

    StatusRequest::StatusRequest()
        : StatusRequest(0, 0)
    {}

    StatusResponse::StatusResponse(uint32_t gameId, uint32_t playerId, GameState state)
        : Response(STATUS_RESPONSE, gameId, playerId), state(state)
    {}

    StatusResponse::StatusResponse()
        : StatusResponse(0, 0, GameState::INITIAL)
    {}

    TurnRequest::TurnRequest(uint32_t gameId, uint32_t playerId, uint8_t x, uint8_t y)
        : Request(TURN_REQUEST, gameId, playerId), x(x), y(y)
    {}

    TurnRequest::TurnRequest(uint32_t gameId, uint32_t playerId, models::Coordinate const &coordinate)
        : TurnRequest(gameId, playerId, static_cast<uint8_t>(coordinate.first), static_cast<uint8_t>(coordinate.second))
    {}

    TurnRequest::TurnRequest()
        : TurnRequest(0, 0, 0, 0)
    {}

    TurnResponse::TurnResponse(uint32_t gameId, uint32_t playerId, models::HitResult hitResult, bool won)
        : Response(TURN_RESPONSE, gameId, playerId), hitResult(hitResult), won(won)
    {}

    TurnResponse::TurnResponse()
        : TurnResponse(0, 0, models::HitResult::MISSED, false)
    {}

    InvalidRequest::InvalidRequest(uint32_t gameId, uint32_t playerId, ErrorType error)
        : Response(INVALID_REQUEST, gameId, playerId), error(error)
    {}

    InvalidRequest::InvalidRequest(uint32_t playerId, ErrorType error)
        : InvalidRequest(0, playerId, error)
    {}

    InvalidRequest::InvalidRequest(ErrorType error)
        : InvalidRequest(0, error)
    {}

    InvalidRequest::InvalidRequest()
        : InvalidRequest(ErrorType::UNKNOWN)
    {}

    ostream& operator<<(ostream &out, ListedGame const &game)
    {
        out << "Game #" << game.id << ":\n";
        out << "  Sea size: " << static_cast<unsigned short>(game.width)
            << "x" << static_cast<unsigned short>(game.height) << "\n";
        out << "  Current players: " << static_cast<unsigned long>(game.players) << "\n";
        return out;
    }

    template <>
    RequestHeader deserialize(std::string const &buf)
    {
        return deserialize<RequestHeader>(buf, true);
    }

    template <>
    ResponseHeader deserialize(string const &buf)
    {
        return deserialize<ResponseHeader>(buf, true);
    }

    template <>
    ListGamesResponse deserialize(string const &buf)
    {
        return deserialize<ListGamesResponse>(buf, true);
    }

    template <>
    vector<ListedGame> deserialize(string const &buf)
    {
        auto response = deserialize<ListGamesResponse>(buf);
        vector<ListedGame> listedGames;

        for (unsigned long i = 0; i < response.games; ++i) {
            auto offset = sizeof response + sizeof(ListedGame) * i;
            auto listedGame = deserialize<ListedGame>(buf.substr(offset, sizeof(ListedGame)));
            listedGames.push_back(listedGame);
        }

        return listedGames;
    }

    template <>
    ShipTypesResponse deserialize(string const &buf)
    {
        return deserialize<ShipTypesResponse>(buf, true);
    }

    template <>
    models::ShipTypes deserialize(string const &buf)
    {
        auto response = deserialize<ShipTypesResponse>(buf);
        models::ShipTypes shipTypes;

        for (unsigned short i = 0; i < response.ships; ++i) {
            auto offset = sizeof response + sizeof(ShipType) * i;
            auto shipType = deserialize<ShipType>(buf.substr(offset, sizeof(ShipType)), true);
            shipTypes[shipType.name] = shipType.size;
        }

        return shipTypes;
    }

    template <>
    MapResponse deserialize(string const &buf)
    {
        return deserialize<MapResponse>(buf, true);
    }

    template <>
    string deserialize(string const &buf)
    {
        auto response = deserialize<MapResponse>(buf);
        return buf.substr(sizeof response, response.size);
    }
}