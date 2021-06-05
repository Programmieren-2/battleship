//
// Created by rne on 08.05.21.
//

#pragma once

#include <string>

#include "Coordinate.h"
#include "Game.h"
#include "Sea.h"

#include "util.h"

namespace models {
    Coordinate readCoordinate(std::string const &prompt);
    Coordinate readCoordinate();
    Orientation readOrientation(string const &prompt);
    Orientation readOrientation();
    Sea readSea(unsigned short width, unsigned short height);

    template <typename PlayerType>
    PlayerType readPlayer(unsigned short width, unsigned short height) {
        static unsigned short playerNum = 0;
        playerNum++;
        string name = util::readWithPrompt("Enter name of player #" + std::to_string(playerNum) + ": ");
        Sea sea = readSea(width, height);
        cout << sea.toString(true);
        return PlayerType(name, sea);
    }

    template <typename PlayerType>
    Game<PlayerType> readGame()
    {
        Game<PlayerType> game;
        unsigned short width = game.getWidth();
        unsigned short height = game.getHeight();

        PlayerType player = readPlayer<PlayerType>(width, height);
        game.addPlayer(player);
        player = readPlayer<PlayerType>(width, height);
        game.addPlayer(player);
        return game;
    }
}