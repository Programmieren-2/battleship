//
// Created by rne on 08.05.21.
//

#pragma once

#include <string>

#include "boostwrap.h"

#include "TCPService.h"

namespace tcp {
    class Server : public TCPService {
    private:
        boost::asio::ip::tcp::acceptor getAcceptor() const;
    protected:
        /*
         * Abstract method to handle requests.
         * Must be overridden by subclasses.
         */
        [[nodiscard]] virtual std::string handleRequest(std::string const &buf) const = 0;
    public:
        using TCPService::TCPService;

        virtual ~Server() = default;

        [[noreturn]] void listen();
    };
}