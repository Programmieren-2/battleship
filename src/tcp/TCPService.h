//
// Created by rne on 08.05.21.
//

#pragma once

#include <string>

#include "boostwrap.h"

namespace tcp {
    class TCPService {
    private:
        boost::asio::ip::address ipAddress;
        unsigned short port;
    protected:
        mutable boost::asio::io_service service;
        mutable boost::asio::ip::tcp::socket socket;
        [[nodiscard]] boost::asio::ip::tcp::endpoint getEndpoint() const;
    public:
        TCPService(boost::asio::ip::address ipAddress, unsigned short port);

        [[nodiscard]] boost::asio::ip::address getAddress() const;
        [[nodiscard]] unsigned int getPort() const;

        [[nodiscard]] std::string receive(string const &terminator) const;
        [[nodiscard]] std::string receive() const;
        void send(string const &message, string const &terminator) const;
        void send(string const &message) const;
    };
}