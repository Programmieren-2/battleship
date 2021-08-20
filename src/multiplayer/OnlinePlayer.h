//
// Created by rne on 15.05.21.
//

#pragma once

#include <chrono>

#include "Player.h"
#include "Sea.h"

namespace multiplayer {
    using Timestamp = std::chrono::time_point<std::chrono::system_clock>;

    class OnlinePlayer : public models::Player {
    private:
        unsigned long id;
        Timestamp keepalive;
    public:
        OnlinePlayer(unsigned long id, std::string const &name, models::Sea const &sea);
        [[nodiscard]] unsigned long getId() const;
        [[nodiscard]] Timestamp getKeepalive() const;

        void updateTimestamp();

        bool operator==(OnlinePlayer const &other) const;
    };
}