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

    typedef struct {
        unsigned long playerId;
        RequestType type;
    } RequestHeader;

    typedef struct {
        ResponseType type;
    } ResponseHeader;

    typedef struct {
        RequestHeader header = {0, RequestType::LOGIN_REQUEST};
        char playerName[32] = "";
    } LoginRequest;

    typedef struct {
        ResponseHeader header = {ResponseType::LOGIN_RESPONSE};
        bool accepted = false;
    } LoginResponse;

    typedef struct {
        RequestHeader header = {0, RequestType::SHIP_TYPES_REQUEST};
    } ShipTypesRequest;

    typedef struct {
        ResponseHeader header = {ResponseType::SHIP_TYPES_RESPONSE};
        unsigned short ships = 0;
    } ShipTypesResponse;

    typedef struct {
        ResponseHeader header = {ResponseType::SHIP_TYPE};
        unsigned short size = 0;
        char name[16] = "";
    } ShipType;

    typedef struct {
        RequestHeader header = {0, RequestType::MAP_REQUEST};
    } MapRequest;

    typedef struct {
        ResponseHeader header = {ResponseType::MAP_RESPONSE};
        unsigned short width = 0;
        unsigned short height = 0;
    } MapResponse;

    typedef struct {
        RequestHeader header = {0, RequestType::SHIP_PLACEMENT_REQUEST};
        char name[16] = "";
        unsigned short x = 0;
        unsigned short y = 0;
        models::Orientation orientation = models::Orientation::X;
    } ShipPlacementRequest;

    typedef struct {
        ResponseHeader header = {ResponseType::SHIP_PLACEMENT_RESPONSE};
        models::PlacementResult result = models::PlacementResult::SUCCESS;
    } ShipPlacementResponse;

    typedef struct {
        RequestHeader header = {0, RequestType::STATUS_REQUEST};
    } StatusRequest;

    typedef struct {
        ResponseHeader header = {ResponseType::STATUS_RESPONSE};
        Status status = Status::WAITING_FOR_OPPONENT;
    } StatusResponse;

    typedef struct {
        RequestHeader header = {0, RequestType::TURN_REQUEST};
        unsigned short x = 0;
        unsigned short y = 0;
    } TurnRequest;

    typedef struct {
        ResponseHeader header = {ResponseType::TURN_RESPONSE};
        bool hit = false;
        bool gameOver = false;
        bool won = false;
    } TurnResponse;

    typedef struct {
        ResponseHeader header = {ResponseType::INVALID_REQUEST};
    } InvalidRequest;

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
        std::string bytes(ptr, ptr + sizeof packet);
        return bytes;
    }
}

#endif //BATTLESHIP_PACKETS_H
