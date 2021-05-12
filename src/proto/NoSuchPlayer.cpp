//
// Created by rne on 12.05.21.
//

#include "NoSuchPlayer.h"

namespace proto {
    NoSuchPlayer::NoSuchPlayer(unsigned long playerId)
            : playerId(playerId)
    {}

    unsigned long NoSuchPlayer::getPlayerId() const noexcept
    {
        return playerId;
    }

    const char *NoSuchPlayer::what() const noexcept
    {
        return "no such player";
    }
}