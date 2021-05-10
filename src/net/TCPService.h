//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_TCPSERVICE_H
#define BATTLESHIP_TCPSERVICE_H

#include <string>

#include <boost/asio.hpp>

#include "Net.h"

namespace net {
    class TCPService {
    private:
        std::string host;
        unsigned short port;
    protected:
        boost::asio::io_service service;
        Socket socket;
    public:
        TCPService(std::string const &host, unsigned short port);
        TCPService(std::string const &host);
        TCPService(unsigned short port);
        TCPService();

        std::string getHost() const;
        unsigned int getPort() const;

        std::string receive(string const &terminator);
        std::string receive();
        boost::system::error_code send(string const &message, string const &terminator);
        boost::system::error_code send(string const &message);
    };
}

#endif //BATTLESHIP_TCPSERVICE_H
