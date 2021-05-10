//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMESERVER_H
#define BATTLESHIP_GAMESERVER_H

#include <map>
#include <string>

#include "Net.h"
#include "Packets.h"
#include "Server.h"

namespace proto {
    class GameServer : public net::Server {
    private:
        std::map<std::string, unsigned short> shipTypes;

        LoginResponse createLoginResponse(bool accepted);
    protected:
        std::string handleRequest(std::string &buf) override;
        std::string processLoginRequest(LoginRequest const &loginRequest);
        std::string processShipTypesRequest(ShipTypesRequest const &shipTypesRequest);
        std::string processMapRequest (MapRequest const &mapRequest);
        std::string processShipPlacementRequest(ShipPlacementRequest const &shipPlacementRequest);
        std::string processStatusRequest(StatusRequest const &statusRequest);
        std::string processTurnRequest(TurnRequest const &turnRequest);
        void appendShipTypes(std::string &buf);
    public:
        GameServer(string const &host, unsigned short port, std::map<std::string, unsigned short> shipTypes);
        GameServer(unsigned short port, std::map<std::string, unsigned short> shipTypes);
        GameServer(string const &host, std::map<std::string, unsigned short> shipTypes);
        GameServer(std::map<std::string, unsigned short> shipTypes);
        GameServer(string const &host, unsigned short port);
        GameServer(unsigned short port);
        GameServer(string const &host);
        GameServer();
    };
}

#endif //BATTLESHIP_GAMESERVER_H
