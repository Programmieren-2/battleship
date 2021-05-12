//
// Created by rne on 12.05.21.
//

#ifndef BATTLESHIP_PLAYER_H
#define BATTLESHIP_PLAYER_H

#include <string>
#include <vector>

namespace proto {
    class Player {
    private:
        unsigned long id;
        std::string name;
    public:
        Player(unsigned long id, std::string name);

        [[nodiscard]] unsigned long getId() const;
        [[nodiscard]] std::string getName() const;
    };

    typedef std::vector<Player> Players;
}

#endif //BATTLESHIP_PLAYER_H
