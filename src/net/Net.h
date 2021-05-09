//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_NET_H
#define BATTLESHIP_NET_H

#include <cstddef>
#include <string>
#include <vector>

#include <boost/asio.hpp>

namespace net {
    using boost::asio::buffer;
    using boost::asio::error::eof;
    using boost::asio::read;
    using boost::asio::read_until;
    using boost::asio::write;

    typedef boost::asio::ip::tcp::acceptor  Acceptor;
    typedef std::vector<std::byte>          Bytes;
    typedef boost::asio::ip::tcp::endpoint  Endpoint;
    typedef boost::system::error_code       ErrorCode;
    typedef boost::asio::io_service         IOService;
    typedef boost::asio::ip::address        IPAddress;
    typedef boost::asio::ip::tcp::socket    Socket;

    namespace Defaults {
        static const std::string    HOST = "127.0.0.1";
        static const unsigned long  PORT = 9000;
    }
}

#endif