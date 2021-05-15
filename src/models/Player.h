//
// Created by rne on 12.05.21.
//

#ifndef BATTLESHIP_PLAYER_H
#define BATTLESHIP_PLAYER_H

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

        [[nodiscard]] models::Sea& getSea();
    };
}

#endif //BATTLESHIP_PLAYER_H
