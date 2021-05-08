//
// Created by rne on 08.05.21.
//
#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "Coordinate.h"
using models::Coordinate;

#include "HitPoint.h"
using models::HitResult;

#include "PlayerBoard.h"
using models::PlayerBoard;
using models::PlayerBoards;

#include "Ship.h"
using models::Ship;
using models::Ships;

#include "bootstrap.h"
using bootstrap::readCoordinate;
using bootstrap::readPlayerBoard;

#include "game.h"

namespace game {
    static bool playNextRound(PlayerBoards &playerBoards)
    {
        static unsigned int round = 0;
        round++;
        unsigned short player = round % 2;
        PlayerBoard &playerBoard = playerBoards[player];
        PlayerBoard &targetBoard = playerBoards[(player + 1) % 2];
        cout << "It's " << playerBoard.getName() << "'s turn." << endl;
        cout << targetBoard.toString();
        Coordinate target = readCoordinate("Specify your target (<x>,<y>): ");

        switch (targetBoard.fireAt(target)) {
            case HitResult::HIT:
                cout << "Hit!" << endl;
                break;
            case HitResult::MISSED:
                cout << "Miss!" << endl;
                break;
            case HitResult::ALREADY_HIT:
                cout << "You already hit here!" << endl;
                break;
        }

        if (targetBoard.allShipsDestroyed()) {
            cout << "All of " << targetBoard.getName() << "'s ships have been destroyed!" << endl;
            cout << playerBoard.getName() << " has won the game.";
            return false;
        }

        return true;
    }

    void spawn()
    {
        PlayerBoards playerBoards = {readPlayerBoard(), readPlayerBoard()};
        while (playNextRound(playerBoards));
    }
}