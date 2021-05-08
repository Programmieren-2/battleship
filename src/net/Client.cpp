//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <boost/asio.hpp>
using boost::asio::buffer;
using boost::asio::buffer_cast;
using boost::asio::error::eof;
using boost::asio::io_service;
using boost::asio::ip::address;
using boost::asio::ip::tcp;
using boost::asio::streambuf;
using boost::asio::write;
using boost::system::error_code;

#include "Defaults.h"
#include "Client.h"

namespace net {
    error_code Client::send(tcp::socket &socket, string const &message)
    {
        error_code error;
        write(socket, buffer(message + "\n"), error);
        return error;
    }

    string Client::receive(tcp::socket &socket)
    {
        streambuf buf;
        read_until(socket, buf, "\n" );
        return buffer_cast<const char*>(buf.data());
    }

    string Client::communicate(string const &host, unsigned int port, string const &message)
    {
        io_service io_service;
        tcp::socket socket(io_service);
        socket.connect(tcp::endpoint(address::from_string(host), port));

        error_code error = send(socket, message);

        if (error && error != eof)
            cerr << "Error while sending message to server." << endl;

        return receive(socket);
    }

    string Client::communicate(unsigned int port, string const &message)
    {
        return communicate(Defaults::HOST, port, message);
    }

    string Client::communicate(string const &host, string const &message)
    {
        return communicate(host, Defaults::PORT, message);
    }

    string Client::communicate(string const &message)
    {
        return communicate(Defaults::HOST, Defaults::PORT, message);
    }
}
