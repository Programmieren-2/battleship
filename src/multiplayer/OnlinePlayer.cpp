//
// Created by rne on 15.05.21.
//

#include <chrono>
using std::chrono::system_clock;

#include <string>
using std::string;

#include "Sea.h"
using models::Sea;

#include "OnlinePlayer.h"

namespace proto {
    OnlinePlayer::OnlinePlayer(unsigned long id, string const &name, Sea const &sea)
        : Player(name, sea), id(id), keepalive(system_clock::now())
    {}

    unsigned long OnlinePlayer::getId() const
    {
        return id;
    }

    Timestamp OnlinePlayer::getKeepalive() const
    {
        return keepalive;
    }

    unsigned long OnlinePlayer::getId()
    {
        return id;
    }

    void OnlinePlayer::updateTimestamp()
    {
        keepalive = system_clock::now();
    }

    bool OnlinePlayer::operator==(const OnlinePlayer &other) const
    {
        return other.getId() == id;
    }
}