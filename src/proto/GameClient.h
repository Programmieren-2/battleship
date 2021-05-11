//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMECLIENT_H
#define BATTLESHIP_GAMECLIENT_H

#include <string>

#include "Net.h"
#include "Client.h"
#include "Messages.h"

namespace proto {
    class GameClient : public net::Client {
    private:
        unsigned long playerId;

        using net::Client::communicate;

        LoginRequest createLoginRequest(std::string const &playerName);
        ShipTypesRequest createShipTypesRequest();

        void processResponse(std::string const &buf);
        void processLoginResponse(LoginResponse const &loginResponse);
        void processShipTypesResponse(std::string const &buf);
    public:
        GameClient(std::string const &host, unsigned short port);
        GameClient(std::string const &host);
        GameClient(unsigned short port);
        GameClient();

        bool sendLoginRequest(std::string const &name);
        void sendShipTypesRequest();
    };
}

#endif //BATTLESHIP_GAMECLIENT_H
