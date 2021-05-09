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
using boost::asio::io_service;
using boost::asio::read_until;
using boost::asio::streambuf;
using boost::asio::write;
using boost::system::error_code;

#include "base64.h"

#include "Net.h"
#include "TCPIO.h"

namespace net {
    TCPIO::TCPIO()
            : service(io_service())
    {}

    string TCPIO::receive(Socket &socket, string const &terminator)
    {
        streambuf buf;
        read_until(socket, buf, terminator);
        string raw = buffer_cast<const char*>(buf.data());
        raw = raw.substr(0, raw.size() - terminator.size());
        return raw;
    }

    string TCPIO::receive(Socket &socket)
    {
        return base64_decode(receive(socket, "\n"));
    }

    error_code TCPIO::send(Socket &socket, string const &message, string const &terminator)
    {
        error_code error;
        write(socket, buffer(message + terminator), error);
        return error;
    }

    error_code TCPIO::send(Socket &socket, string const &message)
    {
        string raw = base64_encode(message);
        return send(socket, raw, "\n");
    }
}