//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_TCPIO_H
#define BATTLESHIP_TCPIO_H

#include <string>

#include <boost/asio.hpp>

#include "Net.h"

namespace net {
    class TCPIO {
    protected:
        boost::asio::io_service service;
    public:
        TCPIO();

        /*
         * Send a raw string followed by a terminator over the socket.
         */
        static std::string receive(Socket &socket, std::string const &terminator);

        /*
         * Send a base64 encoded string followed by a newline character over the socket.
         */
        static std::string receive(Socket &socket);

        /*
         * Receive a raw string from the socket until the terminator is met.
         */
        static boost::system::error_code send(Socket &socket, std::string const &message, std::string const &terminator);

        /*
         * Receive a base64 encoded string from the socket which is terminated by a newline character.
         */
        static boost::system::error_code send(Socket &socket, std::string const &message);
    };
}

#endif //BATTLESHIP_TCPIO_H
