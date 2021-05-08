//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;
using std::endl;

#include "Net.h"
using net::Bytes;
using net::Socket;

#include "Server.h"

#include "Packets.h"
#include "GameServer.h"

namespace proto {
    GameServer::GameServer()
            : Server()
    {}
    Bytes GameServer::processLoginRequest(LoginRequest const &loginRequest)
    {
        LoginResponse response;
        return packetToBytes(response);
    }

    Bytes GameServer::processShipTypesRequest(ShipTypesRequest const &shipTypesRequest)
    {
        ShipTypesResponse response;
        return packetToBytes(response);
    }

    Bytes GameServer::processMapRequest(MapRequest const &mapRequest)
    {
        MapResponse response;
        return packetToBytes(response);
    }

    Bytes GameServer::processShipPlacementRequest(ShipPlacementRequest const &shipPlacementRequest)
    {
        ShipPlacementResponse response;
        return packetToBytes(response);
    }

    void GameServer::appendShipTypes(Bytes &buf)
    {
        return;
    }

    Bytes GameServer::processStatusRequest(StatusRequest const &statusRequest)
    {
        StatusResponse response;
        return packetToBytes(response);
    }

    Bytes GameServer::processTurnRequest(TurnRequest const &turnRequest)
    {
        TurnResponse response;
        return packetToBytes(response);
    }

    Bytes GameServer::processRequest(net::Socket &socket)
    {
        Bytes response, buf = receive(socket, sizeof(RequestHeader));
        RequestHeader header;
        memcpy(&header, &buf[0], buf.size());
        cerr << "Received request " << header.type << " from player " << header.playerId << "." << endl;

        switch (header.type) {
            case RequestType::LOGIN_REQUEST:
                return processLoginRequest(readPacket<LoginRequest>(socket, buf));
            case RequestType::SHIP_TYPES_REQUEST:
                response = processShipTypesRequest(readPacket<ShipTypesRequest>(socket, buf));
                appendShipTypes(response);
                return response;
            case RequestType::MAP_REQUEST:
                return processMapRequest(readPacket<MapRequest>(socket, buf));
            case RequestType::SHIP_PLACEMENT_REQUEST:
                return processShipPlacementRequest(readPacket<ShipPlacementRequest>(socket, buf));
            case RequestType::STATUS_REQUEST:
                return processStatusRequest(readPacket<StatusRequest>(socket, buf));
            case RequestType::TURN_REQUEST:
                return processTurnRequest(readPacket<TurnRequest>(socket, buf));
        }

        return response;
    }

    void GameServer::listen()
    {
        Bytes response;

        while (true) {
            Socket socket = getSocket();
            response = processRequest(socket);
        }
    }
}