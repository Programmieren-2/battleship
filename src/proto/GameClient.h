//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMECLIENT_H
#define BATTLESHIP_GAMECLIENT_H

#include <string>

#include "Ship.h"
#include "Net.h"
#include "Client.h"
#include "Messages.h"

namespace proto {
    class GameClient : public net::Client {
    private:
        unsigned long playerId;

        using net::Client::communicate;

        template <typename RequestType, typename ResponseType>
        ResponseType communicate(RequestType const &request)
        {
            return deserialize<ResponseType>(communicate(serialize(request)));
        }
    public:
        GameClient(std::string const &host, unsigned short port);
        GameClient();

        bool login(std::string const &name);
        models::ShipTypes getShipTypes();
    };
}

#endif //BATTLESHIP_GAMECLIENT_H
