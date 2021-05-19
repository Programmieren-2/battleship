//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_TCPSERVICE_H
#define BATTLESHIP_TCPSERVICE_H

#include <string>

#include <SDKDDKVer.h>
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
        [[nodiscard]] boost::asio::ip::tcp::endpoint getEndpoint() const;
    public:
        TCPService(std::string host, unsigned short port);
        TCPService();

        [[nodiscard]] std::string getHost() const;
        [[nodiscard]] unsigned int getPort() const;

        [[nodiscard]] std::string receive(string const &terminator);
        [[nodiscard]] std::string receive();
        void send(string const &message, string const &terminator);
        void send(string const &message);
    };
}

#endif //BATTLESHIP_TCPSERVICE_H
