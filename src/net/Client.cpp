//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include "Net.h"
#include "Client.h"

namespace net {
    Client::Client()
            : TCPSocket()
    {}

    Bytes Client::communicate(string const &host, unsigned int port, Bytes const &bytes)
    {
        Socket socket = getSocket(host, port);
        socket.connect(Endpoint(IPAddress::from_string(host), port));

        ErrorCode error = send(socket, bytes);

        if (error && error != eof)
            cerr << "Error while sending message to server: " << error.message() << endl;

        return receive(socket);
    }

    Bytes Client::communicate(unsigned int port, Bytes const &message)
    {
        return communicate(Defaults::HOST, port, message);
    }

    Bytes Client::communicate(string const &host, Bytes const &message)
    {
        return communicate(host, Defaults::PORT, message);
    }

    Bytes Client::communicate(Bytes const &message)
    {
        return communicate(Defaults::HOST, Defaults::PORT, message);
    }
}
