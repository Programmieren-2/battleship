//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMESERVER_H
#define BATTLESHIP_GAMESERVER_H

#include <string>

#include "Net.h"
#include "Packets.h"
#include "Server.h"

namespace proto {
    class GameServer : public net::Server {
    private:
        std::string processRequest(net::Socket &socket);
        std::string processLoginRequest(LoginRequest const &loginRequest);
        std::string processShipTypesRequest(ShipTypesRequest const &shipTypesRequest);
        std::string processMapRequest (MapRequest const &mapRequest);
        std::string processShipPlacementRequest(ShipPlacementRequest const &shipPlacementRequest);
        std::string processStatusRequest(StatusRequest const &statusRequest);
        std::string processTurnRequest(TurnRequest const &turnRequest);
        void appendShipTypes(std::string &buf);
    protected:
        using net::Server::getAcceptor;
        using net::Server::getSocket;
    public:
        GameServer();
        [[noreturn]] void listen() override;
    };
}

#endif //BATTLESHIP_GAMESERVER_H
