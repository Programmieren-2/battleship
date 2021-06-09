//
// Created by rne on 15.05.21.
//

#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/config.hpp>

#include "Ship.h"
#include "ShipTypes.h"

namespace models {
    template <typename PlayerType>
    class Game {
    private:
        unsigned short width;
        unsigned short height;
        ShipTypes shipTypes;
        mutable std::array<std::optional<PlayerType>, 2> players;
    public:
        static unsigned short const WIDTH = 10;
        static unsigned short const HEIGHT = 10;
        static ShipTypes const SHIP_TYPES;

        Game(unsigned short width, unsigned short height, ShipTypes shipTypes);
        Game(unsigned short width, unsigned short height);
        Game();

        [[nodiscard]] unsigned short getWidth() const;
        [[nodiscard]] unsigned short getHeight() const;
        [[nodiscard]] ShipTypes getShipTypes() const;
        [[nodiscard]] std::vector<std::reference_wrapper<PlayerType const>> getPlayers() const;
        [[nodiscard]] unsigned short getPlayerCount() const;
        [[nodiscard]] std::optional<std::reference_wrapper<PlayerType const>> getPlayer(unsigned short index) const;
        void removePlayer(PlayerType const &player) const;
        [[nodiscard]] bool hasPlayers() const;
        [[nodiscard]] bool isFull() const;
        [[nodiscard]] bool isEmpty() const;
        bool addPlayer(PlayerType const &player) const;
    };

#include "Game.inl"
}