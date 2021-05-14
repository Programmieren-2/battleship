//
// Created by rne on 12.05.21.
//

#ifndef BATTLESHIP_PLAYER_H
#define BATTLESHIP_PLAYER_H

#include <chrono>
#include <functional>
#include <string>
#include <vector>

#include "PlayerBoard.h"

namespace proto {
    typedef std::chrono::time_point<std::chrono::system_clock> Timestamp;

    class Player {
    private:
        unsigned long id;
        std::string name;
        Timestamp keepalive;
        models::PlayerBoard board;
    public:
        Player(unsigned long id, std::string const &name);

        [[nodiscard]] unsigned long getId() const;
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] Timestamp getKeepalive() const;
        [[nodiscard]] models::PlayerBoard& getBoard();
    };

    typedef std::vector<Player> Players;
    typedef std::reference_wrapper<Player> PlayerRef;
}

#endif //BATTLESHIP_PLAYER_H
