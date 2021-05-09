//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_CLIENT_H
#define BATTLESHIP_CLIENT_H

#include <string>

#include "TCPSocket.h"

namespace net {
    class Client : public TCPSocket {
    protected:
        using TCPSocket::receive;
        using TCPSocket::send;

        virtual Bytes receive(Socket &socket) = 0;
    public:
        Client();
        virtual ~Client() = default;

        Bytes communicate(std::string const &host, unsigned int port, Bytes const &message);
        Bytes communicate(unsigned int port, Bytes const &message);
        Bytes communicate(std::string const &host, Bytes const &message);
        Bytes communicate(Bytes const &message);
    };
}

#endif //BATTLESHIP_CLIENT_H
