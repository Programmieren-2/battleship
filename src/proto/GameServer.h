//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMESERVER_H
#define BATTLESHIP_GAMESERVER_H

#include <string>

#include "Ship.h"
#include "Net.h"
#include "Messages.h"
#include "Server.h"

namespace proto {
    class GameServer : public net::Server {
    private:
        std::map<std::string, unsigned short> shipTypes;

        [[nodiscard]] LoginResponse createLoginResponse(bool accepted) const;
    protected:
        [[nodiscard]] std::string handleRequest(std::string const &buf) const override;
        [[nodiscard]] std::string processLoginRequest(LoginRequest const &loginRequest) const;
        [[nodiscard]] std::string processShipTypesRequest(ShipTypesRequest const &shipTypesRequest) const;
        [[nodiscard]] std::string processMapRequest (MapRequest const &mapRequest) const;
        [[nodiscard]] std::string processShipPlacementRequest(ShipPlacementRequest const &shipPlacementRequest) const;
        [[nodiscard]] std::string processStatusRequest(StatusRequest const &statusRequest) const;
        [[nodiscard]] std::string processTurnRequest(TurnRequest const &turnRequest) const;
        void appendShipTypes(std::string &buf) const;
    public:
        GameServer(string const &host, unsigned short port, models::ShipTypes shipTypes);
        GameServer(string const &host, unsigned short port);
        GameServer(models::ShipTypes shipTypes);
        GameServer();
    };
}

#endif //BATTLESHIP_GAMESERVER_H
