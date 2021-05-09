//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_SERVER_H
#define BATTLESHIP_SERVER_H

#include <string>

#include "Net.h"
#include "TCPSocket.h"

namespace net {
    class Server : public TCPSocket {
    private:
        std::string host;
        unsigned int port;
    protected:
        using TCPSocket::receive;
        using TCPSocket::send;

        Acceptor getAcceptor();
        Socket getSocket();
    public:
        Server(std::string const &host, unsigned int port);
        Server(std::string const &host);
        Server(unsigned int port);
        Server();
        virtual ~Server() = default;

        std::string getHost() const;
        unsigned int getPort() const;

        [[noreturn]] virtual void listen() = 0;
    };
}

#endif //BATTLESHIP_SERVER_H
