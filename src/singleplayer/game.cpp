//
// Created by rne on 08.05.21.
//

#include <array>
using std::array;

#include <iostream>
using std::cerr;
using std::cout;

#include <optional>
using std::optional;

#include <string>
using std::string;

#include "Coordinate.h"
using models::Coordinate;

#include "Game.h"
using models::Game;

#include "HitPoint.h"
using models::HitResult;

#include "Player.h"
using models::Player;

#include "Sea.h"
using models::Sea;
using models::PlayerBoards;

#include "bootstrap.h"
using bootstrap::readCoordinate;
using bootstrap::readGame;

#include "game.h"

namespace game {
    static bool playNextRound(auto &game)
    {
        static unsigned int round = 0;
        round++;
        unsigned short index = round % 2;
        auto playerCandidate = game.getPlayer(index);
        auto opponentCandidate = game.getPlayer((index + 1) % 2);

        if (!playerCandidate.has_value() || !opponentCandidate.has_value()) {
            cerr <<  "Not enough players.\n";
            return false;
        }

        Player &player = playerCandidate.value();
        Player &opponent = opponentCandidate.value();
        Sea &sea = opponent.getSea();
        cout << "It's " << player.getName() << "'s turn.\n";
        cout << sea.toString();
        Coordinate target = readCoordinate("Specify your target (<x>,<y>): ");

        switch (sea.fireAt(target)) {
            case HitResult::HIT:
                cout << "Hit!\n";
                break;
            case HitResult::MISSED:
                cout << "Miss!\n";
                break;
            case HitResult::ALREADY_HIT:
                cout << "You already hit here!\n";
                break;
        }

        if (sea.allShipsDestroyed()) {
            cout << "All of " << opponent.getName() << "'s ships have been destroyed!\n";
            cout << player.getName() << " has won the game.\n";
            return false;
        }

        return true;
    }

    void spawn()
    {
        auto game = readGame<Player>();
        while (playNextRound(game));
    }
}