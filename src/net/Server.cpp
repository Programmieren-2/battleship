//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include "Net.h"
#include "Server.h"

namespace net {
    Server::Server(string const &host, unsigned int port)
            : TCPSocket(), host(host), port(port)
    {}

    Server::Server(string const &host)
            : Server(host, Defaults::PORT)
    {}

    Server::Server(unsigned int port)
            : Server(Defaults::HOST, port)
    {}

    Server::Server()
            : Server(Defaults::HOST, Defaults::PORT)
    {}

    Acceptor Server::getAcceptor()
    {
        return getAcceptor(host, port);
    }

    Socket Server::getSocket()
    {
        return getSocket(host, port);
    }

    string Server::getHost() const
    {
        return host;
    }

    unsigned int Server::getPort() const
    {
        return port;
    }
}