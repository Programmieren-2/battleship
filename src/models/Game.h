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

#include "Models.h"
#include "Ship.h"

namespace models {
    template <typename PlayerType>
    class Game {
    private:
        unsigned short width;
        unsigned short height;
        ShipTypes shipTypes;
        mutable std::array<std::optional<PlayerType>, 2> players;
    public:
        Game(unsigned short width, unsigned short height, ShipTypes shipTypes)
            : width(width), height(height), shipTypes(std::move(shipTypes))
        {}

        Game(unsigned short width, unsigned short height)
            : Game(width, height, Defaults::SHIP_TYPES)
        {}

        Game()
            : Game(Defaults::WIDTH, Defaults::HEIGHT)
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

        [[nodiscard]] std::vector<std::reference_wrapper<PlayerType>> getPlayers() const
        {
            std::vector<std::reference_wrapper<PlayerType>> result;

            for (auto &player : players) {
                if (player.has_value())
                    result.push_back(player.value());
            }

            return result;
        }

        [[nodiscard]] unsigned short getPlayerCount() const
        {
            unsigned short playerCount = 0;

            for (auto const &candidate : players) {
                if (candidate.has_value())
                    playerCount++;
            }

            return playerCount;
        }

        [[nodiscard]] std::optional<PlayerType> getPlayer(unsigned short index) const
        {
            return players.at(index % players.size());
        }

        [[nodiscard]] std::optional<std::reference_wrapper<PlayerType>> getPlayer(unsigned short index)
        {
            return *players.at(index % players.size());
        }

        void removePlayer(PlayerType const &player)
        {
            for (auto &candidate : players) {
                if (candidate.has_value() && candidate.value() == player)
                    candidate.reset();
            }
        }

        [[nodiscard]] bool hasPlayers() const
        {
            return std::any_of(players.begin(), players.end(), [] (auto const &player) { return player.has_value(); });
        }

        [[nodiscard]] bool isFull() const
        {
            return std::all_of(players.begin(), players.end(), [] (auto const &player) { return player.has_value(); });
        }

        bool addPlayer(PlayerType const &newPlayer)
        {
            for (auto &player : players) {
                if (player.has_value())
                    continue;

                player = newPlayer;
                return true;
            }

            return false;
        }
    };
}