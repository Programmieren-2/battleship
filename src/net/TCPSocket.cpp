//
// Created by rne on 08.05.21.
//

#include <cstddef>
using std::byte;

#include <string>
using std::string;

#include <boost/asio.hpp>
using boost::asio::buffer;
using boost::asio::buffer_cast;
using boost::asio::read;
using boost::asio::read_until;
using boost::asio::streambuf;
using boost::asio::transfer_all;
using boost::asio::write;

#include "base64.h"

#include "Net.h"
#include "TCPSocket.h"

namespace net {
    TCPSocket::TCPSocket()
            : service(IOService())
    {}

    string TCPSocket::receive(Socket &socket, string const &sep)
    {
        streambuf buf;
        read_until(socket, buf, sep);
        string raw = buffer_cast<const char*>(buf.data());
        raw = raw.substr(0, raw.size() - sep.size());
        return raw;
    }

    string TCPSocket::receive(Socket &socket)
    {
        return base64_decode(receive(socket, "\n"));
    }

    string TCPSocket::receiveAll(Socket &socket)
    {
        ErrorCode error;
        streambuf receive_buffer;
        read(socket, receive_buffer, transfer_all(), error);
        return buffer_cast<const char*>(receive_buffer.data());
    }

    ErrorCode TCPSocket::send(Socket &socket, string const &message, string const &sep)
    {
        ErrorCode error;
        write(socket, buffer(message + sep), error);
        return error;
    }

    ErrorCode TCPSocket::send(Socket &socket, string const &message)
    {
        string raw = base64_encode(message);
        return send(socket, raw, "\n");
    }
}