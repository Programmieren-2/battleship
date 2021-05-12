//
// Created by rne on 12.05.21.
//

#include <chrono>
using std::chrono::system_clock;

#include <string>
using std::string;

#include "PlayerBoard.h"
using models::PlayerBoard;

#include "Player.h"

namespace proto {
    Player::Player(unsigned long id, string const &name)
            : id(id), name(name), keepalive(system_clock::now()), board(PlayerBoard(name))
    {}

    unsigned long Player::getId() const
    {
        return id;
    }

    string Player::getName() const
    {
        return name;
    }

    Timestamp Player::getKeepalive() const
    {
        return keepalive;
    }

    PlayerBoard Player::getBoard()
    {
        return board;
    }
}