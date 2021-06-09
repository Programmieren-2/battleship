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
using boost::asio::io_service;
using boost::asio::ip::address;
using boost::asio::read_until;
using boost::asio::streambuf;
using boost::asio::write;
using boost::system::error_code;
using Endpoint = boost::asio::ip::tcp::endpoint;
using Socket = boost::asio::ip::tcp::socket;

#include "SocketError.h"
#include "Service.h"

namespace tcp {
    Service::Service(address ipAddress, unsigned short port)
        : ipAddress(move(ipAddress)), port(port), service(io_service()), socket(Socket(service))
    {}

    address Service::getAddress() const
    {
        return ipAddress;
    }

    unsigned int Service::getPort() const
    {
        return port;
    }

    Endpoint Service::getEndpoint() const
    {
        return Endpoint(ipAddress, port);
    }

    string Service::receive(string const &terminator) const
    {
        streambuf buf;
        error_code error;
        read_until(socket, buf, terminator, error);

        if (BOOST_UNLIKELY(error.failed()))
            throw SocketError(error);

        string raw = buffer_cast<const char*>(buf.data());
        return raw.substr(0, raw.size() - terminator.size());
    }

    string Service::receive() const
    {
        return base64_decode(receive("\n"));
    }

    void Service::send(string const &message, string const &terminator) const
    {
        error_code error;
        write(socket, buffer(message + terminator), error);

        if (BOOST_UNLIKELY(error.failed()))
            throw SocketError(error);
    }

    void Service::send(string const &message) const
    {
        return send(base64_encode(message), "\n");
    }
}