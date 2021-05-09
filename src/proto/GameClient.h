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
    class GameClient : public net::Client{
    private:
        void processResponse(string &buf);
        void processLoginResponse(LoginResponse const &loginResponse);
    protected:
        using net::Client::receive;
    public:
        bool sendLoginRequest(std::string const &name);
    };
}

#endif //BATTLESHIP_GAMECLIENT_H
