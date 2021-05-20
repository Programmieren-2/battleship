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
using boost::asio::ip::tcp;
using boost::asio::read_until;
using boost::asio::streambuf;
using boost::asio::write;
using boost::system::error_code;

#include "Net.h"
#include "SocketError.h"
#include "TCPService.h"

namespace net {
    TCPService::TCPService(IPAddress ipAddress, unsigned short port)
            : ipAddress(move(ipAddress)), port(port), service(io_service()), socket(Socket(service))
    {}

    IPAddress TCPService::getAddress() const
    {
        return ipAddress;
    }

    unsigned int TCPService::getPort() const
    {
        return port;
    }

    tcp::endpoint TCPService::getEndpoint() const
    {
        return tcp::endpoint(ipAddress, port);
    }

    string TCPService::receive(string const &terminator)
    {
        streambuf buf;
        error_code error;
        read_until(socket, buf, terminator, error);

        if (BOOST_UNLIKELY(error && error != eof))
            throw SocketError(error);

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
            throw SocketError(error);
    }

    void TCPService::send(string const &message)
    {
        return send(base64_encode(message), "\n");
    }
}