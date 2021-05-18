//
// Created by rne on 15.05.21.
//

#ifndef BATTLESHIP_GAME_H
#define BATTLESHIP_GAME_H

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
        typedef std::optional<PlayerType> OptionalPlayer;
        typedef std::array<OptionalPlayer, 2> OptionalPlayers;
        typedef std::reference_wrapper<PlayerType> PlayerRef;
        typedef std::optional<PlayerRef> OptionalPlayerRef;
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

            for (auto const &candidate : players) {
                if (candidate.has_value())
                    result.push_back(candidate.value());
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

        std::vector<PlayerRef> accessPlayers()
        {
            std::vector<PlayerRef> result;

            for (auto &candidate : players) {
                if (candidate.has_value())
                    result.push_back(candidate.value());
            }

            return result;
        }

        OptionalPlayerRef getPlayer(unsigned short index)
        {
            OptionalPlayerRef player;

            try {
                player = *players.at(index % 2);
            } catch (std::out_of_range&) {
                return player;
            }

            return player;
        }

        void removePlayer(PlayerType const &player)
        {
            for (auto &candidate : players) {
                if (candidate.has_value() && candidate.value() == player)
                    candidate.reset();
            }
        }

        [[nodiscard]] bool hasPlayers() const {
            return any_of(players.begin(), players.end(), [](auto &player) { return player.has_value(); });
        }

        bool addPlayer(PlayerType const &newPlayer) {
            for (auto &player : players) {
                if (BOOST_UNLIKELY(player.has_value()))
                    continue;

                player = newPlayer;
                return true;
            }

            return false;
        }
    };
}

#endif //BATTLESHIP_GAME_H
