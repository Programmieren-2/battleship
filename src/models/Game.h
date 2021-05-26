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
        using OptionalPlayer = std::optional<PlayerType>;
        using OptionalPlayers = std::array<OptionalPlayer, 2>;
        using PlayerReference = std::reference_wrapper<PlayerType>;
    private:
        unsigned short width;
        unsigned short height;
        ShipTypes shipTypes;
        OptionalPlayers players;
    public:
        Game(unsigned short width, unsigned short height, ShipTypes shipTypes, OptionalPlayers players)
            : width(width), height(height), shipTypes(std::move(shipTypes)), players(players)
        {}

        Game(unsigned short width, unsigned short height, ShipTypes shipTypes)
            : Game(width, height, shipTypes, OptionalPlayers())
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

        [[nodiscard]] std::vector<PlayerType> getPlayers() const
        {
            std::vector<PlayerType> result;

            for (auto const &player : players) {
                if (player.has_value())
                    result.push_back(player.value());
            }

            return result;
        }

        [[nodiscard]] std::vector<PlayerReference> getPlayers()
        {
            std::vector<PlayerReference> result;

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

        std::optional<PlayerType> getPlayer(unsigned short index) const
        {
            try {
                return players.at(index % 2);
            } catch (std::out_of_range&) {
                return std::optional<PlayerType>();
            }
        }

        std::optional<PlayerReference> getPlayer(unsigned short index)
        {
            try {
                return *players.at(index % 2);
            } catch (std::out_of_range&) {
                return std::optional<PlayerReference>();
            }
        }

        void removePlayer(PlayerType const &player)
        {
            for (auto &candidate : players) {
                if (candidate.has_value() && candidate.value() == player)
                    candidate.reset();
            }
        }

        [[nodiscard]] bool hasPlayers() const {
            return any_of(players.begin(), players.end(), [](auto const &player) { return player.has_value(); });
        }

        bool addPlayer(PlayerType const &newPlayer) {
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