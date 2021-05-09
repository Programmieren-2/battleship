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
    public:
        Client();

        std::string communicate(std::string const &host, unsigned int port, std::string const &message);
        std::string communicate(unsigned int port, std::string const &message);
        std::string communicate(std::string const &host, std::string const &message);
        std::string communicate(std::string const &message);
    };
}

#endif //BATTLESHIP_CLIENT_H
