//
// Created by rne on 09.06.21.
//

#pragma once

#include <stdexcept>
#include <string>

namespace multiplayer {
    class CLIError : std::exception {
    private:
        std::string message;
    public:
        explicit CLIError(std::string message);
        [[nodiscard]] char const * what() const noexcept override;

        friend std::ostream& operator<<(std::ostream &out, CLIError const &error);
    };
}