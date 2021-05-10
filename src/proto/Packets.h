//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_PACKETS_H
#define BATTLESHIP_PACKETS_H

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
        unsigned long gameId;
        unsigned long playerId;
        RequestType type;
    } RequestHeader;

    typedef struct {
        unsigned long gameId;
        unsigned long playerId;
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
        unsigned short ships = 0;
    } ShipTypesResponse;

    /*
     * ShipType is a special appendix packet to ShipTypesResponse and thus does not need a header.
     */
    typedef struct shipType {
        char name[32] = "";
        unsigned short size = 0;
    } ShipType;

    typedef struct mapRequest{
        RequestHeader header = {0, 0, RequestType::MAP_REQUEST};
    } MapRequest;

    typedef struct mapResponse {
        ResponseHeader header = {0, 0, ResponseType::MAP_RESPONSE};
        unsigned short width = 0;
        unsigned short height = 0;
    } MapResponse;

    typedef struct shipPlacementRequest {
        RequestHeader header = {0, 0, RequestType::SHIP_PLACEMENT_REQUEST};
        char name[32] = "";
        unsigned short x = 0;
        unsigned short y = 0;
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
        unsigned short x = 0;
        unsigned short y = 0;
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
     * Convert a byte string into a packet.
     */
    template <typename PacketType>
    PacketType deserialize(std::string &buf, bool partialProcessing = false)
    {
        PacketType packet;
        size_t bufSize = buf.length();
        size_t packetSize = sizeof packet;

        if (packetSize > bufSize)
            std::cerr << "Packet larger than buffer size. Partial read: " << (packetSize - bufSize) << std::endl;

        if (packetSize < bufSize && !partialProcessing)
            std::cerr << "Packet smaller than buffer size. Bytes unprocessed: " << (bufSize - packetSize) << std::endl;

        auto ptr = reinterpret_cast<char*>(&buf[0]);
        memcpy(&packet, ptr, (packetSize < bufSize) ? packetSize : bufSize);
        return packet;
    }

    /*
     * Convert a packet into a byte string.
     */
    template <typename PacketType>
    std::string serialize(PacketType &packet)
    {
        auto ptr = reinterpret_cast<char*>(&packet);
        return std::string(ptr, ptr + sizeof packet);
    }
}

#endif //BATTLESHIP_PACKETS_H
