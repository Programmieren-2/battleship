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

#include "Client.h"

namespace net {
    std::string Client::communicate(std::string const &bytes)
    {
        socket.connect(tcp::endpoint(address::from_string(getHost()), getPort()));
        error_code error = send(bytes);

        if (error)
            cerr << "Error while sending message to server: " << error.message() << endl;

        string response = receive();
        socket.close();
        return response;
    }
}
