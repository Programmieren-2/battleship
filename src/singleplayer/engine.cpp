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

#include "bootstrap.h"
using models::readCoordinate;
using models::readGame;

#include "engine.h"

namespace engine {
    static bool enoughPlayers(optional<Player> const &player, optional<Player> const &opponent)
    {
        if (player && opponent)
            return true;

        cerr << "Not enough players.\n";
        return false;
    }

    static void takeShot(Sea &sea, Coordinate const &target)
    {
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
    }

    static bool gameOver(Player const &player, Player const &opponent)
    {
        if (!opponent.getSea().allShipsDestroyed())
            return false;

        cout << "All of " << opponent.getName() << "'s ships have been destroyed!\n";
        cout << player.getName() << " has won the game.\n";
        return true;
    }

    static bool makeTurn(Player const &player, Player const &opponent)
    {
        Coordinate target = readCoordinate("Specify your target (<x>,<y>): ");
        takeShot(opponent.getSea(), target);
        return !gameOver(player, opponent);
    }

    static bool playRound(Player const &player, Player const &opponent)
    {
        cout << "It's " << player.getName() << "'s turn.\n";
        cout << opponent.getSea();
        return makeTurn(player, opponent);
    }

    static bool playNextRound(Game<Player> &game)
    {
        static unsigned int round = 0;
        round++;
        auto playerCandidate = game.getPlayer(round % 2);
        auto opponentCandidate = game.getPlayer((round + 1) % 2);

        if (!enoughPlayers(playerCandidate, opponentCandidate))
            return false;

        return playRound(*playerCandidate, *opponentCandidate);
    }

    void spawn()
    {
        auto game = readGame<Player>();
        while (playNextRound(game));
    }
}