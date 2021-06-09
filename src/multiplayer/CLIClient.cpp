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

#include "HitResult.h"
using models::HitResult;

#include "CLIError.h"

#include "util.h"
using util::isExitCommand;
using util::joinStrings;
using util::readCommandLine;

#include "CLIClient.h"

namespace multiplayer {
    void CLIClient::help(string const &command, vector<string> const &args)
    {
        if (args.empty()) {
            cout << "Available commands:\n";

            for (auto &[cmd, _] : COMMANDS)
                cout << "* " << cmd << "\n";

            return;
        }

        if (args.size() == 1)
            throw CLIError("Help on specific commands is not yet implemented. Sorry.");

        cerr << "Use '" << command << "' to get general help.\n";
        cerr << "Use '" << command << " <command>' to get help on a specific command.\n";
    }

    void CLIClient::listGames(string const &command, vector<string> const &args)
    {
        if (!args.empty())
            throw CLIError("Command '" + command + "' does not expect any arguments.");

        for (auto const &game : listGames())
            cout << game;
    }

    void CLIClient::newGame(string const &command, vector<string> const &args)
    {
        unsigned short width, height;
        unsigned long newGameId;

        if (args.empty()) {
            newGameId = newGame();
        } else if (args.size() == 2) {
            try {
                width = static_cast<unsigned short>(stoul(args[0]));
            } catch (invalid_argument const &) {
                throw CLIError("Invalid width.");
            }

            try {
                height = static_cast<unsigned short>(stoul(args[1]));
            } catch (invalid_argument const &) {
                throw CLIError("Invalid height.");
            }

            newGameId = newGame(width, height);
        } else {
            throw CLIError("Usage: '" + command + "' or '" + command + " <width> <height>'.");
        }

        cout << "New game id: " << newGameId << "\n";
    }

    void CLIClient::joinGame(string const &command, vector<string> const &args)
    {
        if (args.size() != 2)
            throw CLIError("Usage: '" + command + " <gameID> <playerName>' to join a game.");

        unsigned long newGameId = 0;

        try {
            newGameId = stoul(args[0]);
        } catch (invalid_argument const &) {
            throw CLIError("Invalid game ID.");
        }

        if (!join(newGameId, args[1]))
            throw CLIError("Failed to join game.");

        cout << "Joined game #" << newGameId << ".\n";
    }

    void CLIClient::logout(string const &command, vector<string> const &args)
    {
        if (!args.empty())
            throw CLIError("Command '" + command + "' expects no arguments.");

        logout();
    }

    void CLIClient::getShipTypes(string const &command, vector<string> const &args)
    {
        if (!args.empty())
            throw CLIError("Command '" + command + "' expects not arguments.");

        cout << "Available ship types:\n";

        for (auto &[name, size] : getShipTypes())
            cout << "* " << name << " (" << size << ")\n";
    }

    void CLIClient::placeShip(string const &command, vector<string> const &args)
    {
        if (args.size() < 4)
            throw CLIError("Usage: '" + command + " <type...> <x> <y> (x|y)");

        vector<string> typeElements(args.begin(), args.begin() + args.size() - 3);
        string type = joinStrings(typeElements, " ");
        string strX = args[args.size() - 3];
        string strY = args[args.size() - 2];

        auto anchorPoint = Coordinate::fromString(strX, strY);
        if (!anchorPoint)
            throw CLIError("Invalid coordinates: " + strX + "x" + strY);

        string orientationStr = args[args.size() - 1];
        Orientation orientation;

        if (orientationStr == "x")
            orientation = Orientation::X;
        else if (orientationStr == "y")
            orientation = Orientation::Y;
        else
            throw CLIError("Invalid orientation: '" + orientationStr + "'. Use 'x' or 'y'.");

        BasicShip ship(type, *anchorPoint, orientation);

        switch (placeShip(ship)) {
            case SHIP_PLACED:
                cout << type << " placed at " << strX << "x" << strY << " facing " << orientationStr << ".\n";
                break;
            case INVALID_SHIP_TYPE:
                cerr << "Invalid ship type: " << type << ".\n";
                break;
            case ALREADY_PLACED:
                cerr << "You already placed your " << type << ".\n";
                break;
            case COLLISION:
                cerr << type << " collides with another ship.\n";
                break;
            case OUTSIDE_SEA_BOUNDS:
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
            throw CLIError("Usage: '" + command + " (own)'.");
        }

        cout << getMap(own);
    }

    void CLIClient::makeTurn(string const &command, vector<string> const &args)
    {
        if (args.size() != 2)
            throw CLIError("Usage: '" + command + " <x> <y>'.");

        optional<Coordinate> target = Coordinate::fromString(args[0], args[1]);

        if (!target)
            throw CLIError("Invalid coordinate: " + args[0] + "x" + args[1]);

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
            throw CLIError("Command '" + command + "' does not expect any arguments.");

        switch (getStatus()) {
            case INITIAL:
                cout << "Game has not yet started.\n";
                break;
            case WAITING_FOR_PLAYERS:
                cout << "Waiting for an opponent to join.\n";
                break;
            case WAITING_FOR_OPPONENTS_SHIP_PLACEMENTS:
                cout << "Waiting for opponent to place their ships.\n";
                break;
            case WAITING_FOR_YOUR_SHIP_PLACEMENTS:
                cout << "Waiting for you to place your ships.\n";
                break;
            case WAITING_FOR_OPPONENTS_TURN:
                cout << "Waiting for opponent to make their turn.\n";
                break;
            case WAITING_FOR_YOUR_TURN:
                cout << "Waiting for you to make your turn.\n";
                break;
            case ABANDONED:
                cout << "Game is abandoned.\n";
                break;
        }
    }

    void CLIClient::handleCommand(string const &command, vector<string> const &args)
    {
        switch (getCommand(command)) {
            case INVALID:
                cerr << "Invalid command. Type 'help' to list available commands.\n";
                break;
            case HELP:
                help(command, args);
                break;
            case LIST_GAMES:
                listGames(command, args);
                break;
            case NEW_GAME:
                newGame(command, args);
                break;
            case JOIN:
                joinGame(command, args);
                break;
            case LOGOUT:
                logout(command, args);
                break;
            case GET_SHIP_TYPES:
                getShipTypes(command, args);
                break;
            case PLACE_SHIP:
                placeShip(command, args);
                break;
            case GET_MAP:
                getMap(command, args);
                break;
            case MAKE_TURN:
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
        } catch (ProtocolError const &error) {
            handleProtocolError(error);
        } catch (CLIError const &error) {
            cerr << error << "\n";
        }
    }

    Command CLIClient::getCommand(string const &command)
    {
        try {
            return COMMANDS.at(command);
        } catch (out_of_range const &) {
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