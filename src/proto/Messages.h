//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_MESSAGES_H
#define BATTLESHIP_MESSAGES_H

#include <cstring>
#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include "Constants.h"
#include "Coordinate.h"
#include "Net.h"
#include "PlayerBoard.h"

#include "BufferSizeMismatch.h"

namespace proto {
    enum GameState {
        WAITING_FOR_PLAYERS,
        WAITING_FOR_YOUR_SHIP_PLACEMENTS,
        WAITING_FOR_OPPONENTS_SHIP_PLACEMENTS,
        WAITING_FOR_YOUR_TURN,
        WAITING_FOR_OPPONENTS_TURN,
    };

    enum RequestType {
        NOOP,
        LOGIN_REQUEST,
        SHIP_TYPES_REQUEST,
        MAP_REQUEST,
        SHIP_PLACEMENT_REQUEST,
        STATUS_REQUEST,
        TURN_REQUEST,
    };

    enum ResponseType {
        INVALID_REQUEST,
        LOGIN_RESPONSE,
        SHIP_TYPES_RESPONSE,
        SHIP_TYPE,
        MAP_RESPONSE,
        SHIP_PLACEMENT_RESPONSE,
        STATUS_RESPONSE,
        TURN_RESPONSE,
    };

#pragma pack(push, 1)

    struct RequestHeader {
        RequestType type;
        uint32_t gameId;
        uint32_t playerId;

        RequestHeader(RequestType type, unsigned int gameId, unsigned int playerId)
            : type(type), gameId(static_cast<uint32_t>(gameId)), playerId(static_cast<uint32_t>(playerId))
        {}

        RequestHeader(RequestType type)
                : RequestHeader(type, 0, 0)
        {}

        RequestHeader()
                : RequestHeader(RequestType::NOOP)
        {}
    };

    struct ResponseHeader {
        ResponseType type;
        uint32_t gameId;
        uint32_t playerId;

        ResponseHeader(ResponseType type, unsigned int gameId, unsigned int playerId)
            : type(type), gameId(static_cast<uint32_t>(gameId)), playerId(static_cast<uint32_t>(playerId))
        {}

        ResponseHeader(ResponseType type)
        : ResponseHeader(type, 0, 0)
        {}

        ResponseHeader()
                : ResponseHeader(ResponseType::INVALID_REQUEST)
        {}
    };

    typedef struct loginRequest {
        RequestHeader header = {RequestType::LOGIN_REQUEST};
        char playerName[32] = "";
    } LoginRequest;

    typedef struct loginResponse {
        ResponseHeader header = {ResponseType::LOGIN_RESPONSE};
        bool accepted = false;
    } LoginResponse;

    typedef struct shipTypesRequest {
        RequestHeader header = {RequestType::SHIP_TYPES_REQUEST};
    } ShipTypesRequest;

    typedef struct shipTypesResponse {
        ResponseHeader header = {ResponseType::SHIP_TYPES_RESPONSE};
        uint8_t ships = 0;
    } ShipTypesResponse;

    /*
     * ShipType is a special appendix packet to ShipTypesResponse and thus does not need a header.
     */
    typedef struct shipType {
        char name[32] = "";
        uint8_t size = 0;
    } ShipType;

    typedef struct mapRequest{
        RequestHeader header = {RequestType::MAP_REQUEST};
    } MapRequest;

    typedef struct mapResponse {
        ResponseHeader header = {ResponseType::MAP_RESPONSE};
        uint8_t width = 0;
        uint8_t height = 0;
        uint32_t size = 0;  // Followed by so many ASCII chars for ASCII representation of the map
    } MapResponse;

    typedef struct shipPlacementRequest {
        RequestHeader header = {RequestType::SHIP_PLACEMENT_REQUEST};
        char name[32] = "";
        uint8_t x = 0;
        uint8_t y = 0;
        models::Orientation orientation = models::Orientation::X;
    } ShipPlacementRequest;

    typedef struct shipPlacementResponse {
        ResponseHeader header = {ResponseType::SHIP_PLACEMENT_RESPONSE};
        models::PlacementResult result = models::PlacementResult::SUCCESS;
    } ShipPlacementResponse;

    typedef struct statusRequest {
        RequestHeader header = {RequestType::STATUS_REQUEST};
    } StatusRequest;

    typedef struct statusResponse {
        ResponseHeader header = {ResponseType::STATUS_RESPONSE};
        GameState status = GameState::WAITING_FOR_PLAYERS;
    } StatusResponse;

    typedef struct turnRequest {
        RequestHeader header = {RequestType::TURN_REQUEST};
        uint8_t x = 0;
        uint8_t y = 0;
    } TurnRequest;

    typedef struct turnResponse {
        ResponseHeader header = {ResponseType::TURN_RESPONSE};
        bool hit = false;
        bool gameOver = false;
        bool won = false;
    } TurnResponse;

    typedef struct invalidRequest {
        ResponseHeader header = {ResponseType::INVALID_REQUEST};
    } InvalidRequest;

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
            throw BufferSizeMismatch("Message size exceeds buffer size.");

        if (msgSize < bufSize && !partialProcessing)
            throw BufferSizeMismatch("Buffer size exceeds message size.");

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

#endif //BATTLESHIP_MESSAGES_H
