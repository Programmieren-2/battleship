//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_SERVER_H
#define BATTLESHIP_SERVER_H

#include <string>

#include <boost/asio.hpp>

#include "Net.h"
#include "TCPIO.h"

namespace net {
    class Server : public TCPIO {
    private:
        std::string host;
        unsigned int port;
    protected:
        using TCPIO::receive;
        using TCPIO::send;

        boost::asio::ip::tcp::acceptor getAcceptor();
        Socket getSocket();
    public:
        Server(std::string const &host, unsigned short port);
        Server(std::string const &host);
        Server(unsigned short port);
        Server();
        virtual ~Server() = default;

        std::string getHost() const;
        unsigned int getPort() const;

        /*
         * Abstract method to spawn the server and let it serve requests.
         * Must be overridden by subclasses.
         */
        [[noreturn]] virtual void listen() = 0;
    };
}

#endif //BATTLESHIP_SERVER_H
