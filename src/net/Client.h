//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_CLIENT_H
#define BATTLESHIP_CLIENT_H

#include <string>

#include "TCPIO.h"

namespace net {
    class Client : public TCPIO {
    protected:
        using TCPIO::receive;
        using TCPIO::send;
    public:
        Client();

        /*
         * Sends a base64 encoded string to the server and returns a base64 encoded string as response.
         */
        std::string communicate(std::string const &host, unsigned int port, std::string const &message);
        std::string communicate(unsigned int port, std::string const &message);
        std::string communicate(std::string const &host, std::string const &message);
        std::string communicate(std::string const &message);
    };
}

#endif //BATTLESHIP_CLIENT_H
