//
// Created by rne on 12.05.21.
//

#pragma once

#include <string>

#include "Sea.h"

namespace models {
    class Player {
    private:
        std::string name;
        models::Sea sea;
    public:
        Player(std::string name, models::Sea sea);

        [[nodiscard]] std::string getName() const;
        [[nodiscard]] models::Sea getSea() const;
        
        [[nodiscard]] models::Sea& getSea();
    };
}