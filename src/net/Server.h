//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_SERVER_H
#define BATTLESHIP_SERVER_H

#include <string>

#include "boostwrap.h"

#include "TCPService.h"

namespace net {
    class Server : public TCPService {
    private:
        boost::asio::ip::tcp::acceptor getAcceptor();
    protected:
        /*
         * Abstract method to handle requests.
         * Must be overridden by subclasses.
         */
        [[nodiscard]] virtual std::string handleRequest(std::string const &buf) = 0;
    public:
        using TCPService::TCPService;

        virtual ~Server() = default;

        [[noreturn]] void listen();
    };
}

#endif //BATTLESHIP_SERVER_H
