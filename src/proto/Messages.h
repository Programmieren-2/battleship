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
        MAP_RESPONSE,
        SHIP_PLACEMENT_RESPONSE,
        STATUS_RESPONSE,
        TURN_RESPONSE,
    };

#pragma pack(push, 1)

    struct RequestHeader {
        RequestType type = RequestType::NOOP;
        uint32_t gameId = 0;
        uint32_t playerId = 0;
    };

    struct ResponseHeader {
        ResponseType type = ResponseType::INVALID_REQUEST;
        uint32_t gameId = 0;
        uint32_t playerId = 0;
    };

    struct LoginRequest {
        RequestHeader header = {RequestType::LOGIN_REQUEST};
        char playerName[32] = "";
    };

    struct LoginResponse {
        ResponseHeader header = {ResponseType::LOGIN_RESPONSE};
        bool accepted = false;
    };

    struct ShipTypesRequest {
        RequestHeader header = {RequestType::SHIP_TYPES_REQUEST};
    };

    struct ShipTypesResponse {
        ResponseHeader header = {ResponseType::SHIP_TYPES_RESPONSE};
        uint8_t ships = 0;
    };

    /*
     * ShipType is a special appendix packet to ShipTypesResponse and thus does not need a header.
     */
    struct ShipType {
        char name[32] = "";
        uint8_t size = 0;
    };

    struct MapRequest {
        RequestHeader header = {RequestType::MAP_REQUEST};
        bool own = false;
    };

    struct MapResponse {
        ResponseHeader header = {ResponseType::MAP_RESPONSE};
        uint8_t width = 0;
        uint8_t height = 0;
        uint32_t size = 0;  // Followed by so many ASCII chars for ASCII representation of the map
    };

    struct ShipPlacementRequest {
        RequestHeader header = {RequestType::SHIP_PLACEMENT_REQUEST};
        char type[32] = "";
        uint8_t x = 0;
        uint8_t y = 0;
        models::Orientation orientation = models::Orientation::X;
    };

    struct ShipPlacementResponse {
        ResponseHeader header = {ResponseType::SHIP_PLACEMENT_RESPONSE};
        models::PlacementResult result = models::PlacementResult::SUCCESS;
    };

    struct StatusRequest {
        RequestHeader header = {RequestType::STATUS_REQUEST};
    };

    struct StatusResponse {
        ResponseHeader header = {ResponseType::STATUS_RESPONSE};
        GameState status = GameState::WAITING_FOR_PLAYERS;
    };

    struct TurnRequest {
        RequestHeader header = {RequestType::TURN_REQUEST};
        uint8_t x = 0;
        uint8_t y = 0;
    };

    struct TurnResponse {
        ResponseHeader header = {ResponseType::TURN_RESPONSE};
        bool hit = false;
        bool gameOver = false;
        bool won = false;
    };

    struct InvalidRequest {
        ResponseHeader header = {ResponseType::INVALID_REQUEST};
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
