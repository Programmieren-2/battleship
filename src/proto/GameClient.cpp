//
// Created by rne on 08.05.21.
//

#include "Client.h"
using net::Client;

#include "Net.h"
using net::Bytes;
using net::Socket;

#include "GameClient.h"

namespace proto {
    Bytes GameClient::receive(net::Socket &socket)
    {
        return Client::receive(socket, 0);
    }
}