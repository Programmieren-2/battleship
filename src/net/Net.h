//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_NET_H
#define BATTLESHIP_NET_H

#include <string>

#include <boost/asio.hpp>

namespace net {
    typedef boost::asio::ip::tcp::socket    Socket;

    namespace Defaults {
        static const std::string    HOST = "127.0.0.1";
        static const unsigned long  PORT = 9000;
    }
}

#endif