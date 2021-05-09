//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include <boost/asio.hpp>
using boost::asio::ip::address;
using boost::asio::ip::tcp;

#include "Net.h"
#include "Server.h"

namespace net {
    Server::Server(string const &host, unsigned int port)
            : TCPIO(), host(host), port(port)
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

    tcp::acceptor Server::getAcceptor()
    {
        return tcp::acceptor(service, tcp::endpoint(address::from_string(host), port));
    }

    Socket Server::getSocket()
    {
        Socket socket(service);
        tcp::acceptor acceptor = getAcceptor();
        acceptor.accept(socket);
        return socket;
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