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
    protected:
        using net::Client::receive;
        net::Bytes receive(net::Socket &socket) override;
    public:
        bool sendLoginRequest(std::string const &name);
        net::Bytes processLoginResponse(LoginResponse const &loginResponse);
    };
}

#endif //BATTLESHIP_GAMECLIENT_H
