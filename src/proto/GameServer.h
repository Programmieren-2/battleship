//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMESERVER_H
#define BATTLESHIP_GAMESERVER_H

#include "Net.h"
#include "Packets.h"
#include "Server.h"

namespace proto {
    class GameServer : public net::Server {
    private:
        net::Bytes processRequest(net::Socket &socket);
        net::Bytes processLoginRequest(LoginRequest const &loginRequest);
        net::Bytes processShipTypesRequest(ShipTypesRequest const &shipTypesRequest);
        net::Bytes processMapRequest (MapRequest const &mapRequest);
        net::Bytes processShipPlacementRequest(ShipPlacementRequest const &shipPlacementRequest);
        net::Bytes processStatusRequest(StatusRequest const &statusRequest);
        net::Bytes processTurnRequest(TurnRequest const &turnRequest);
        void appendShipTypes(net::Bytes &buf);
    protected:
        using net::Server::getAcceptor;
        using net::Server::getSocket;
    public:
        GameServer();
        [[noreturn]] void listen() override;
    };
}

#endif //BATTLESHIP_GAMESERVER_H
