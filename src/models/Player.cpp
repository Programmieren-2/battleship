//
// Created by rne on 12.05.21.
//

#include <chrono>
using std::chrono::system_clock;

#include <string>
using std::string;

#include <utility>
using std::move;

#include "Sea.h"
#include "Player.h"

namespace models {
    Player::Player(string name, Sea sea)
        : name(move(name)), sea(move(sea))
    {}

    string Player::getName() const
    {
        return name;
    }

    Sea& Player::getSea() const
    {
        return sea;
    }
}