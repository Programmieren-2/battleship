//
// Created by rne on 08.05.21.
//

#pragma once

#include <string>

#include "boostwrap.h"

#include "Net.h"

namespace net {
    class TCPService {
    private:
        IPAddress ipAddress;
        unsigned short port;
    protected:
        boost::asio::io_service service;
        Socket socket;
        [[nodiscard]] boost::asio::ip::tcp::endpoint getEndpoint() const;
    public:
        TCPService(IPAddress ipAddress, unsigned short port);

        [[nodiscard]] IPAddress getAddress() const;
        [[nodiscard]] unsigned int getPort() const;

        [[nodiscard]] std::string receive(string const &terminator);
        [[nodiscard]] std::string receive();
        void send(string const &message, string const &terminator);
        void send(string const &message);
    };
}