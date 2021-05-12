//
// Created by rne on 12.05.21.
//

#include <string>
using std::string;

#include <utility>
using std::move;

#include "Player.h"

namespace proto {
    Player::Player(unsigned long id, string name)
            : id(id), name(move(name))
    {}

    unsigned long Player::getId() const
    {
        return id;
    }

    string Player::getName() const
    {
        return name;
    }
}