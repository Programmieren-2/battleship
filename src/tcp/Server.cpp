//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include "boostwrap.h"
using Acceptor = boost::asio::ip::tcp::acceptor;

#include "Server.h"

namespace tcp {
    Acceptor Server::getAcceptor() const
    {
        return Acceptor(service, getEndpoint());
    }

    void Server::listen()
    {
        Acceptor acceptor = getAcceptor();
        string buf;

        while (true) {
            acceptor.accept(socket);
            send(handleRequest(receive()));
            socket.close();
        }
    }
}