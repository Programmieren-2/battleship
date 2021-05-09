//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <boost/asio.hpp>
using boost::asio::ip::address;
using boost::asio::ip::tcp;
using boost::system::error_code;

#include "Net.h"
#include "Client.h"

namespace net {
    Client::Client()
            : TCPIO()
    {}

    std::string Client::communicate(string const &host, unsigned int port, std::string const &bytes)
    {
        Socket socket(service);
        socket.connect(tcp::endpoint(address::from_string(host), port));

        error_code error = send(socket, bytes);

        if (error)
            cerr << "Error while sending message to server: " << error.message() << endl;

        return receive(socket);
    }

    std::string Client::communicate(unsigned int port, std::string const &message)
    {
        return communicate(Defaults::HOST, port, message);
    }

    std::string Client::communicate(string const &host, std::string const &message)
    {
        return communicate(host, Defaults::PORT, message);
    }

    std::string Client::communicate(std::string const &message)
    {
        return communicate(Defaults::HOST, Defaults::PORT, message);
    }
}
