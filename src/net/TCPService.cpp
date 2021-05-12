//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include <boost/asio.hpp>
using boost::asio::buffer;
using boost::asio::buffer_cast;
using boost::asio::error::eof;
using boost::asio::io_service;
using boost::asio::ip::address;
using boost::asio::ip::tcp;
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

    tcp::endpoint TCPService::getEndpoint() const
    {
        return tcp::endpoint(address::from_string(host), port);
    }

    string TCPService::receive(string const &terminator)
    {
        streambuf buf;
        error_code error;
        read_until(socket, buf, terminator, error);

        if (error && error != eof)
            throw error;

        string raw = buffer_cast<const char*>(buf.data());
        return raw.substr(0, raw.size() - terminator.size());
    }

    string TCPService::receive()
    {
        return base64_decode(receive("\n"));
    }

    void TCPService::send(string const &message, string const &terminator)
    {
        error_code error;
        write(socket, buffer(message + terminator), error);

        if (error)
            throw error;
    }

    void TCPService::send(string const &message)
    {
        return send(base64_encode(message), "\n");
    }
}