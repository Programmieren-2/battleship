//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include "Server.h"

namespace net {
    tcp::acceptor Server::getAcceptor()
    {
        return tcp::acceptor(service, getEndpoint());
    }

    void Server::listen()
    {
        tcp::acceptor acceptor = getAcceptor();
        string buf;

        while (true) {
            acceptor.accept(socket);
            send(handleRequest(receive()));
            socket.close();
        }
    }
}