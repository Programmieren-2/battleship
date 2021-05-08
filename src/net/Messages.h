//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_MESSAGES_H
#define BATTLESHIP_MESSAGES_H

#include "Constants.h"
#include "Coordinate.h"
#include "PlayerBoard.h"

namespace messages {
    enum Status {
        WAITING_FOR_OPPONENT, WAITING_FOR_YOU
    };
    enum Type {
        LOGIN_REQUEST,
        LOGIN_RESPONSE,
        SHIP_TYPES_REQUEST,
        SHIP_TYPES_RESPONSE,
        SHIP_TYPE,
        MAP_REQUEST,
        MAP_RESPONSE,
        SHIP_PLACEMENT_REQUEST,
        SHIP_PLACEMENT_RESPONSE,
        STATUS_REQUEST,
        STATUS_RESPONSE,
        TURN_REQUEST,
        TURN_RESPONSE
    };

    typedef struct {
        unsigned long id;
        Type type;
    } PacketHeader;

    namespace requests {
        static unsigned long packetCounter;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::LOGIN_REQUEST};
            char playerName[32] = "";
        } LoginRequest;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::SHIP_TYPES_REQUEST};
        } ShipTypesRequest;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::MAP_REQUEST};
        } MapRequest;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::SHIP_PLACEMENT_REQUEST};
            char name[16] = "";
            unsigned short x = 0;
            unsigned short y = 0;
            models::Orientation orientation = models::Orientation::X;
        } ShipPlacementRequest;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::STATUS_REQUEST};
        } StatusRequest;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::TURN_REQUEST};
            unsigned short x = 0;
            unsigned short y = 0;
        } TurnRequest;
    }

    namespace responses {
        static unsigned long packetCounter;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::LOGIN_RESPONSE};
            bool accepted = false;
        } LoginResponse;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::SHIP_TYPES_RESPONSE};
            unsigned short ships = 0;
        } ShipTypesResponse;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::SHIP_TYPE};
            unsigned short size = 0;
            char name[16] = "";
        } ShipType;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::MAP_RESPONSE};
            unsigned short width = 0;
            unsigned short height = 0;
        } MapResponse;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::SHIP_PLACEMENT_RESPONSE};
            models::PlacementResult result = models::PlacementResult::SUCCESS;
        } ShipPlacementResponse;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::STATUS_RESPONSE};
            Status status = Status::WAITING_FOR_OPPONENT;
        } StatusResponse;

        typedef struct {
            PacketHeader header = {packetCounter++, Type::TURN_RESPONSE};
            bool hit = false;
        } TurnResponse;
    }
}

#endif //BATTLESHIP_MESSAGES_H
