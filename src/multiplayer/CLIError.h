//
// Created by rne on 09.06.21.
//

#pragma once

#include <stdexcept>
#include <string>

#include "Command.h"

namespace multiplayer {
    class CLIError : public std::exception {
    private:
        std::string message;
    public:
        explicit CLIError(std::string message);
        explicit CLIError(Command const &command);
        [[nodiscard]] char const * what() const noexcept override;

        friend std::ostream& operator<<(std::ostream &out, CLIError const &error);
    };
}