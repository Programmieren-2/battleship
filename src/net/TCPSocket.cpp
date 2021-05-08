//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include "Net.h"
#include "TCPSocket.h"

namespace net {
    TCPSocket::TCPSocket()
            : service(IOService())
    {}

    Acceptor TCPSocket::getAcceptor(string const &host, unsigned int port)
    {
        return Acceptor(service, Endpoint(IPAddress::from_string(host), port));
    }

    Socket TCPSocket::getSocket(string const &host, unsigned int port)
    {
        Socket socket(service);
        Acceptor acceptor = getAcceptor(host, port);
        acceptor.accept(socket);
        return socket;
    }

    Bytes TCPSocket::receive(Socket &socket, size_t size)
    {
        Bytes buf;
        read(socket, buffer(buf, size));
        return buf;
    }

    ErrorCode TCPSocket::send(Socket &socket, Bytes bytes)
    {
        ErrorCode error;
        write(socket, buffer(bytes), error);
        return error;
    }
}