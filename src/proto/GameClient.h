//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMECLIENT_H
#define BATTLESHIP_GAMECLIENT_H

#include <string>

#include "Net.h"
#include "Client.h"
#include "Packets.h"

namespace proto {
    class GameClient : public net::Client {
    private:
        unsigned long playerId;

        LoginRequest createLoginRequest(std::string const &playerName);
        ShipTypesRequest createShipTypesRequest();

        void processResponse(std::string &buf);
        void processLoginResponse(LoginResponse const &loginResponse);
        void processShipTypesResponse(std::string &buf);
    protected:
        using net::Client::receive;
    public:
        GameClient(unsigned long playerId);
        GameClient();

        bool sendLoginRequest(std::string const &name);
        void sendShipTypesRequest();
    };
}

#endif //BATTLESHIP_GAMECLIENT_H
