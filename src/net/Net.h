//
// Created by rne on 08.05.21.
//

#pragma once

#include "boostwrap.h"

namespace net {
    typedef boost::asio::ip::address        IPAddress;
    typedef boost::asio::ip::tcp::socket    Socket;

    namespace Defaults {
        static const char *ADDRESS = "127.0.0.1";
        static const unsigned short PORT = 9000;
    }
}