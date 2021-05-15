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
    OnlinePlayer::OnlinePlayer(unsigned long id, string const &name, Sea &sea)
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

    void OnlinePlayer::updateTimestamp()
    {
        keepalive = system_clock::now();
    }
}