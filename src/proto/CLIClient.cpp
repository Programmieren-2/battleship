//
// Created by rne on 15.05.21.
//

#include <iostream>
using std::cerr;
using std::cin;
using std::cout;

#include <numeric>
using std::accumulate;

#include <optional>
using std::optional;

#include <stdexcept>
using std::invalid_argument;
using std::out_of_range;

#include <string>
using std::stoul;
using std::string;

#include <vector>
using std::vector;

#include "BasicShip.h"
using models::BasicShip;

#include "Coordinate.h"
using models::Coordinate;
using models::Orientation;

#include "Models.h"
using models::HitResult;
using models::PlacementResult;

#include "util.h"
using util::isExitCommand;
using util::joinStrings;
using util::readCommandLine;

#include "CLIClient.h"

namespace proto {
    void CLIClient::help(string const &command, vector<string> const &args)
    {
        if (args.empty()) {
            cout << "Available commands:\n";

            for (auto &[cmd, _] : COMMANDS)
                cout << "* " << cmd << "\n";

            return;
        }

        if (args.size() == 1) {
            cerr << "Help on specific commands is not yet implemented. Sorry.\n";
            return;
        }

        cerr << "Use '" << command << "' to get general help.\n";
        cerr << "Use '" << command << " <command>' to get help on a specific command.\n";
    }

    void CLIClient::listGames(string const &command, vector<string> const &args)
    {
        if (!args.empty())
            cerr << "Command '" << command << "' does not expect any arguments.\n";

        for (auto const &game : listGames()) {
            cout << "Game #" << game.id << ":\n";
            cout << "  Sea size: " << static_cast<unsigned short>(game.width)
                    << "x" << static_cast<unsigned short>(game.height) << "\n";
            cout << "  Current players: " << static_cast<unsigned long>(game.players) << "\n";
        }
    }

    void CLIClient::newGame(string const &command, vector<string> const &args)
    {
        unsigned short width, height;

        if (args.empty()) {
            width = models::Defaults::WIDTH;
            height = models::Defaults::HEIGHT;
        } else if (args.size() == 2) {
            try {
                width = static_cast<unsigned short>(stoul(args[0]));
            } catch (invalid_argument&) {
                cerr << "Invalid width.\n";
                return;
            }

            try {
                height = static_cast<unsigned short>(stoul(args[1]));
            } catch (invalid_argument&) {
                cerr << "Invalid height.\n";
                return;
            }
        } else {
            cerr << "Invalid amount of parameters.\n";
            cerr << "Use '" << command << "' or '" << command << " <width> <height>'.\n";
            return;
        }

        unsigned long newGameId = newGame(width, height);
        cout << "New game id: " << newGameId << "\n";
    }

    void CLIClient::joinGame(string const &command, vector<string> const &args)
    {
        if (args.size() != 2) {
            cerr << "Usage: '" << command << " <gameID> <playerName>' to join a game.\n";
            return;
        }

        unsigned long newGameId = 0;

        try {
            newGameId = stoul(args[0]);
        } catch (invalid_argument&) {
            cerr << "Invalid game ID.\n";
            return;
        }

        if (join(newGameId, args[1]))
            cout << "Joined game #" << newGameId << ".\n";
        else
            cerr << "Failed to join game.\n";
    }

    void CLIClient::logout(string const &command, vector<string> const &args)
    {
        if (!args.empty())
            cerr << "Command '" << command << "' expects no arguments.\n";

        logout();
    }

    void CLIClient::getShipTypes(string const &command, vector<string> const &args)
    {
        if (!args.empty())
            cerr << "Command '" << command << "' expects not arguments.\n";

        cout << "Available ship types:\n";

        for (auto &[name, size] : getShipTypes())
            cout << "* " << name << " (" << size << ")\n";
    }

    void CLIClient::placeShip(string const &command, vector<string> const &args)
    {
        if (args.size() < 4) {
            cerr << "Usage: '" << command << " <type...> <x> <y> (x|y)\n";
            return;
        }

        vector<string> typeElements(args.begin(), args.begin() + args.size() - 3);
        string type = joinStrings(typeElements, " ");
        string strX = args[args.size() - 3];
        string strY = args[args.size() - 2];

        auto anchorPoint = Coordinate::fromString(strX, strY);
        if (!anchorPoint) {
            cerr << "Invalid coordinates: " << strX << "x" << strY << "\n";
            return;
        }

        string orientationStr = args[args.size() - 1];
        Orientation orientation;

        if (orientationStr == "x")
            orientation = Orientation::X;
        else if (orientationStr == "y")
            orientation = Orientation::Y;
        else {
            cerr << "Invalid orientation: '" << orientationStr << "'. Use 'x' or 'y'.\n";
            return;
        }

        BasicShip ship(type, *anchorPoint, orientation);

        switch (placeShip(ship)) {
            case PlacementResult::SUCCESS:
                cout << type << " placed at " << strX << "x" << strY << " facing " << orientationStr << ".\n";
                break;
            case PlacementResult::INVALID_SHIP_TYPE:
                cerr << "Invalid ship type: " << type << ".\n";
                break;
            case PlacementResult::ALREADY_PLACED:
                cerr << "You already placed your " << type << ".\n";
                break;
            case PlacementResult::COLLISION:
                cerr << type << " collides with another ship.\n";
                break;
            case PlacementResult::NOT_ON_BOARD:
                cerr << type << " is not (entirely) on the sea board.\n";
                break;
        }
    }

    void CLIClient::getMap(string const &command, vector<string> const &args)
    {
        bool own;

        if (args.empty()) {
            own = false;
        } else if (args.size() == 1 && args[0] == "own") {
            own = true;
        } else {
            cerr << "Usage: '" << command << " (own)'.\n";
            return;
        }

        cout << getMap(own);
    }

    void CLIClient::makeTurn(string const &command, vector<string> const &args)
    {
        if (args.size() != 2) {
            cerr << "Usage: '" << command << " <x> <y>'.\n";
            return;
        }

        optional<Coordinate> target = Coordinate::fromString(args[0], args[1]);

        if (!target) {
            cerr << "Invalid coordinate: " << args[0] << "x" << args[1] << "\n";
            return;
        }

        switch (fireAt(*target)) {
            case HitResult::HIT:
                cout << "Hit!\n";
                break;
            case HitResult::MISSED:
                cout << "Missed.\n";
                break;
            case HitResult::ALREADY_HIT:
                cerr << "You already hit here!\n";
                break;
        }
    }

    void CLIClient::getStatus(string const &command, vector<string> const &args)
    {
        if (!args.empty())
            cerr << "Command '" << command << "' does not expect any arguments.\n";

        switch (getStatus()) {
            case GameState::INITIAL:
                cout << "Game has not yet started.\n";
                break;
            case GameState::WAITING_FOR_PLAYERS:
                cout << "Waiting for an opponent to join.\n";
                break;
            case GameState::WAITING_FOR_OPPONENTS_SHIP_PLACEMENTS:
                cout << "Waiting for opponent to place their ships.\n";
                break;
            case GameState::WAITING_FOR_YOUR_SHIP_PLACEMENTS:
                cout << "Waiting for you to place your ships.\n";
                break;
            case GameState::WAITING_FOR_OPPONENTS_TURN:
                cout << "Waiting for opponent to make their turn.\n";
                break;
            case GameState::WAITING_FOR_YOUR_TURN:
                cout << "Waiting for you to make your turn.\n";
                break;
        }
    }

    void CLIClient::handleCommand(string const &command, vector<string> const &args)
    {
        switch (getCommand(command)) {
            case Command::INVALID:
                cerr << "Invalid command. Type 'help' to list available commands.\n";
                break;
            case Command::HELP:
                help(command, args);
                break;
            case Command::LIST_GAMES:
                listGames(command, args);
                break;
            case Command::NEW_GAME:
                newGame(command, args);
                break;
            case Command::JOIN:
                joinGame(command, args);
                break;
            case Command::LOGOUT:
                logout(command, args);
                break;
            case Command::GET_SHIP_TYPES:
                getShipTypes(command, args);
                break;
            case Command::PLACE_SHIP:
                placeShip(command, args);
                break;
            case Command::GET_MAP:
                getMap(command, args);
                break;
            case Command::MAKE_TURN:
                makeTurn(command, args);
                break;
            case GET_STATUS:
                getStatus(command, args);
                break;
        }
    }

    void CLIClient::handleProtocolError(ProtocolError const &error)
    {
        cerr << "[ERROR] ";

        switch (error.getType()) {
            case NO_SUCH_GAME:
                cerr << "No such game.\n";
                break;
            case NO_SUCH_PLAYER:
                cerr << "No such player.\n";
                break;
            case NO_OPPONENT:
                cerr << "No opponent.\n";
                break;
            case NOT_YOUR_TURN:
                cerr << "It is not your turn.\n";
                break;
            case OPPONENT_NOT_READY:
                cerr << "Your opponent is not ready.\n";
                break;
            case UNEXPECTED_RESPONSE_TYPE:
                cerr << "Unexpected response type. Complain to Richard.\n";
                break;
            case ALREADY_LOGGED_IN:
                cerr << "You are already logged into a game.\n";
                break;
            case GAME_OVER:
                cerr << "The game has ended.\n";
                break;
            default:
                cerr << "Unknown error: " << error.getType() << "\n";
                break;
        }
    }

    void CLIClient::handleCommandSafely(string const &command, vector<string> const &args)
    {
        try {
            handleCommand(command, args);
        } catch (ProtocolError &error) {
            handleProtocolError(error);
        }
    }

    Command CLIClient::getCommand(string const &command)
    {
        try {
            return COMMANDS.at(command);
        } catch (out_of_range&) {
            return Command::INVALID;
        }
    }

    void CLIClient::loop()
    {
        vector<string> commandLine;

        while (!cin.eof()) {
            commandLine = readCommandLine("% ");

            if (commandLine.empty())
                continue;

            string command = commandLine[0];

            if (isExitCommand(command))
                break;

            vector<string> args(commandLine.begin() + 1, commandLine.end());
            handleCommandSafely(command, args);
        }

        teardown();
    }
}