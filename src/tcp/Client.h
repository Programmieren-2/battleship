//
// Created by rne on 08.05.21.
//

#pragma once

#include <string>

#include "TCPService.h"

namespace tcp {
    class Client : public TCPService {
    private:
        using TCPService::receive;
        using TCPService::send;
    public:
        using TCPService::TCPService;

        std::string communicate(std::string const &message) const;
    };
}