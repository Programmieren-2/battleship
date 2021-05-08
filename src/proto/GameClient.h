//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMECLIENT_H
#define BATTLESHIP_GAMECLIENT_H

#include "Net.h"
#include "Client.h"

namespace proto {
    class GameClient : public net::Client{
    protected:
        net::Bytes receive(net::Socket &socket) override;
    };
}

#endif //BATTLESHIP_GAMECLIENT_H
