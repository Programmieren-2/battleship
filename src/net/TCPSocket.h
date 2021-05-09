//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_TCPSOCKET_H
#define BATTLESHIP_TCPSOCKET_H

#include <string>

#include "Net.h"

namespace net {
    class TCPSocket {
    protected:
        IOService service;

        static std::string receive(Socket &socket, std::string const &sep);
        static std::string receive(Socket &socket);
        static std::string receiveAll(Socket &socket);
        static ErrorCode send(Socket &socket, std::string const &message, std::string const &sep);
        static ErrorCode send(Socket &socket, std::string const &message);
    public:
        TCPSocket();
    };
}

#endif //BATTLESHIP_TCPSOCKET_H
