//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_CLIENT_H
#define BATTLESHIP_CLIENT_H

#include <string>

#include "TCPService.h"

namespace net {
    class Client : public TCPService {
    private:
        using TCPService::receive;
        using TCPService::send;
    protected:
        /*
         * Sends a base64 encoded string to the server and returns a base64 encoded string as response.
         */
        std::string communicate(std::string const &message);
    public:
        using TCPService::TCPService;
    };
}

#endif //BATTLESHIP_CLIENT_H
