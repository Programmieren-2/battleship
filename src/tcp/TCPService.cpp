//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include <utility>
using std::move;

#include "base64.h"

#include "boostwrap.h"
using boost::asio::buffer;
using boost::asio::buffer_cast;
using boost::asio::error::eof;
using boost::asio::io_service;
using boost::asio::ip::address;
using boost::asio::read_until;
using boost::asio::streambuf;
using boost::asio::write;
using boost::system::error_code;
using Endpoint = boost::asio::ip::tcp::endpoint;
using Socket = boost::asio::ip::tcp::socket;

#include "SocketError.h"
#include "TCPService.h"

namespace tcp {
    TCPService::TCPService(address ipAddress, unsigned short port)
        : ipAddress(move(ipAddress)), port(port), service(io_service()), socket(Socket(service))
    {}

    address TCPService::getAddress() const
    {
        return ipAddress;
    }

    unsigned int TCPService::getPort() const
    {
        return port;
    }

    Endpoint TCPService::getEndpoint() const
    {
        return Endpoint(ipAddress, port);
    }

    string TCPService::receive(string const &terminator) const
    {
        streambuf buf;
        error_code error;
        read_until(socket, buf, terminator, error);

        if (BOOST_UNLIKELY(error && error != eof))
            throw SocketError(error);

        string raw = buffer_cast<const char*>(buf.data());
        return raw.substr(0, raw.size() - terminator.size());
    }

    string TCPService::receive() const
    {
        return base64_decode(receive("\n"));
    }

    void TCPService::send(string const &message, string const &terminator) const
    {
        error_code error;
        write(socket, buffer(message + terminator), error);

        if (BOOST_UNLIKELY(error))
            throw SocketError(error);
    }

    void TCPService::send(string const &message) const
    {
        return send(base64_encode(message), "\n");
    }
}