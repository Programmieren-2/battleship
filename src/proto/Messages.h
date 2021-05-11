//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_MESSAGES_H
#define BATTLESHIP_MESSAGES_H

#include <iostream>

#include <boost/asio.hpp>

#include "Constants.h"
#include "Coordinate.h"
#include "Net.h"
#include "PlayerBoard.h"

namespace proto {
    enum Status {
        WAITING_FOR_OPPONENT, WAITING_FOR_YOU
    };
    enum RequestType {
        LOGIN_REQUEST,
        SHIP_TYPES_REQUEST,
        MAP_REQUEST,
        SHIP_PLACEMENT_REQUEST,
        STATUS_REQUEST,
        TURN_REQUEST
    };
    enum ResponseType {
        LOGIN_RESPONSE,
        SHIP_TYPES_RESPONSE,
        SHIP_TYPE,
        MAP_RESPONSE,
        SHIP_PLACEMENT_RESPONSE,
        STATUS_RESPONSE,
        TURN_RESPONSE,
        INVALID_REQUEST
    };

#pragma pack(push, 1)
    typedef struct {
        uint32_t gameId;
        uint32_t playerId;
        RequestType type;
    } RequestHeader;

    typedef struct {
        uint32_t gameId;
        uint32_t playerId;
        ResponseType type;
    } ResponseHeader;

    typedef struct loginRequest {
        RequestHeader header = {0, 0, RequestType::LOGIN_REQUEST};
        char playerName[32] = "";
    } LoginRequest;

    typedef struct loginResponse {
        ResponseHeader header = {0, 0, ResponseType::LOGIN_RESPONSE};
        bool accepted = false;
    } LoginResponse;

    typedef struct shipTypesRequest {
        RequestHeader header = {0, 0, RequestType::SHIP_TYPES_REQUEST};
    } ShipTypesRequest;

    typedef struct shipTypesResponse {
        ResponseHeader header = {0, 0, ResponseType::SHIP_TYPES_RESPONSE};
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
        RequestHeader header = {0, 0, RequestType::MAP_REQUEST};
    } MapRequest;

    typedef struct mapResponse {
        ResponseHeader header = {0, 0, ResponseType::MAP_RESPONSE};
        uint8_t width = 0;
        uint8_t height = 0;
    } MapResponse;

    typedef struct shipPlacementRequest {
        RequestHeader header = {0, 0, RequestType::SHIP_PLACEMENT_REQUEST};
        char name[32] = "";
        uint8_t x = 0;
        uint8_t y = 0;
        models::Orientation orientation = models::Orientation::X;
    } ShipPlacementRequest;

    typedef struct shipPlacementResponse {
        ResponseHeader header = {0, 0, ResponseType::SHIP_PLACEMENT_RESPONSE};
        models::PlacementResult result = models::PlacementResult::SUCCESS;
    } ShipPlacementResponse;

    typedef struct statusRequest {
        RequestHeader header = {0, 0, RequestType::STATUS_REQUEST};
    } StatusRequest;

    typedef struct statusResponse {
        ResponseHeader header = {0, 0, ResponseType::STATUS_RESPONSE};
        Status status = Status::WAITING_FOR_OPPONENT;
    } StatusResponse;

    typedef struct turnRequest {
        RequestHeader header = {0, 0, RequestType::TURN_REQUEST};
        uint8_t x = 0;
        uint8_t y = 0;
    } TurnRequest;

    typedef struct turnResponse {
        ResponseHeader header = {0, 0, ResponseType::TURN_RESPONSE};
        bool hit = false;
        bool gameOver = false;
        bool won = false;
    } TurnResponse;

    typedef struct invalidRequest {
        ResponseHeader header = {0, 0, ResponseType::INVALID_REQUEST};
    } InvalidRequest;

#pragma pack(pop)

    /*
     * Convert a byte string into a message struct.
     */
    template <typename Message>
    Message deserialize(std::string &buf, bool partialProcessing = false)
    {
        Message msg;
        size_t bufSize = buf.length();
        size_t msgSize = sizeof msg;

        if (msgSize > bufSize)
            std::cerr << "Packet larger than buffer size. Partial read: " << (msgSize - bufSize) << std::endl;

        if (msgSize < bufSize && !partialProcessing)
            std::cerr << "Packet smaller than buffer size. Bytes unprocessed: " << (bufSize - msgSize) << std::endl;

        auto ptr = reinterpret_cast<char*>(&buf[0]);
        memcpy(&msg, ptr, (msgSize < bufSize) ? msgSize : bufSize);
        return msg;
    }

    /*
     * Convert a message struct into a byte string.
     */
    template <typename Message>
    std::string serialize(Message &msg)
    {
        auto ptr = reinterpret_cast<char*>(&msg);
        return std::string(ptr, ptr + sizeof msg);
    }
}

#endif //BATTLESHIP_MESSAGES_H
