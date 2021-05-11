//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;

#include <string>
using std::string;

#include <boost/asio.hpp>
using boost::asio::buffer;
using boost::asio::buffer_cast;
using boost::asio::error::eof;
using boost::asio::io_service;
using boost::asio::read_until;
using boost::asio::streambuf;
using boost::asio::write;
using boost::system::error_code;

#include "base64.h"

#include "Net.h"
#include "TCPService.h"

namespace net {
    TCPService::TCPService(string const &host, unsigned short port)
            : host(host), port(port), service(io_service()), socket(Socket(service))
    {}

    TCPService::TCPService(string const &host)
            : TCPService(host, Defaults::PORT)
    {}

    TCPService::TCPService(unsigned short port)
            : TCPService(Defaults::HOST, port)
    {}

    TCPService::TCPService()
            : TCPService(Defaults::HOST, Defaults::PORT)
    {}

    string TCPService::getHost() const
    {
        return host;
    }

    unsigned int TCPService::getPort() const
    {
        return port;
    }

    string TCPService::receive(string const &terminator)
    {
        streambuf buf;
        error_code error;
        read_until(socket, buf, terminator, error);

        if (error == eof)
            cerr << "Reached EOF while reading message.\n";

        string raw = buffer_cast<const char*>(buf.data());
        raw = raw.substr(0, raw.size() - terminator.size());
        return raw;
    }

    string TCPService::receive()
    {
        return base64_decode(receive("\n"));
    }

    error_code TCPService::send(string const &message, string const &terminator)
    {
        error_code error;
        write(socket, buffer(message + terminator), error);
        return error;
    }

    error_code TCPService::send(string const &message)
    {
        return send(base64_encode(message), "\n");
    }
}