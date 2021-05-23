//
// Created by rne on 08.05.21.
//

#pragma once

#include "boostwrap.h"

namespace net {
    using IPAddress = boost::asio::ip::address;
    using Socket = boost::asio::ip::tcp::socket;

    namespace Defaults {
        static const char *ADDRESS = "127.0.0.1";
        static const unsigned short PORT = 9000;
    }
}