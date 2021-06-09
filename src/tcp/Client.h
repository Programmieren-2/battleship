//
// Created by rne on 08.05.21.
//

#pragma once

#include <string>

#include "Service.h"

namespace tcp {
    class Client : public Service {
    private:
        using Service::receive;
        using Service::send;
    public:
        using Service::Service;

        std::string communicate(std::string const &message) const;
    };
}