//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_TCPSOCKET_H
#define BATTLESHIP_TCPSOCKET_H

#include <string>

#include "Net.h"

namespace net {
    class TCPSocket {
    private:
        IOService service;
    protected:
        Acceptor getAcceptor(std::string const &host, unsigned int port);
        Socket getSocket(std::string const &host, unsigned int port);
        static Bytes receive(Socket &socket, size_t size);
        static ErrorCode send(Socket &socket, Bytes bytes);
    public:
        TCPSocket();
    };
}

#endif //BATTLESHIP_TCPSOCKET_H
