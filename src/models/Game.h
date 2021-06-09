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

        Game(unsigned short width, unsigned short height, ShipTypes shipTypes)
            : width(width), height(height), shipTypes(std::move(shipTypes))
        {}

        Game(unsigned short width, unsigned short height)
            : Game(width, height, SHIP_TYPES)
        {}

        Game()
            : Game(WIDTH, HEIGHT)
        {}

        [[nodiscard]] unsigned short getWidth() const
        {
            return width;
        }

        [[nodiscard]] unsigned short getHeight() const
        {
            return height;
        }

        [[nodiscard]] ShipTypes getShipTypes() const
        {
            return shipTypes;
        }

        [[nodiscard]] std::vector<std::reference_wrapper<PlayerType const>> getPlayers() const
        {
            std::vector<std::reference_wrapper<PlayerType const>> result;

            for (auto const &candidate : players) {
                if (candidate)
                    result.push_back(*candidate);
            }

            return result;
        }

        [[nodiscard]] unsigned short getPlayerCount() const
        {
            unsigned short playerCount = 0;

            for (auto const &candidate : players) {
                if (candidate)
                    playerCount++;
            }

            return playerCount;
        }

        [[nodiscard]] std::optional<std::reference_wrapper<PlayerType const>> getPlayer(unsigned short index) const
        {
            return *players.at(index % players.size());
        }

        void removePlayer(PlayerType const &player) const
        {
            for (auto &candidate : players) {
                if (candidate && *candidate == player)
                    candidate.reset();
            }
        }

        [[nodiscard]] bool hasPlayers() const
        {
            return std::ranges::any_of(players, [] (auto const &player) { return player.has_value(); });
        }

        [[nodiscard]] bool isFull() const
        {
            return std::ranges::all_of(players, [] (auto const &player) { return player.has_value(); });
        }

        [[nodiscard]] bool isEmpty() const
        {
            return std::ranges::none_of(players, [] (auto const &player) { return player.has_value(); });
        }

        bool addPlayer(PlayerType const &player) const
        {
            for (auto &candidate : players) {
                if (candidate)
                    continue;

                candidate = player;
                return true;
            }

            return false;
        }
    };

    template <typename PlayerType>
    ShipTypes const Game<PlayerType>::SHIP_TYPES = {
        {"Speed boat", static_cast<unsigned short>(2)},
        {"Destroyer", static_cast<unsigned short>(3)},
        {"Submarine", static_cast<unsigned short>(3)},
        {"Battleship", static_cast<unsigned short>(4)},
        {"Aircraft Carrier", static_cast<unsigned short>(5)},
    };
}