//
// Created by rne on 15.05.21.
//

#pragma once

#include <string>
#include <vector>

#include "Command.h"
#include "GameClient.h"
#include "ProtocolError.h"

namespace multiplayer {
    const std::map<std::string, Command> COMMANDS = make_map({
        {CommandCode::HELP, "help", {"[<command>]"}, "Show help page."},
        {CommandCode::LIST_GAMES, "list", {}, "List available games."},
        {CommandCode::NEW_GAME, "new", {"[<width> <height>]"}, "Create a new game."},
        {CommandCode::JOIN, "join", {"<gameID>", "<playerName>"}, "Join a game."},
        {CommandCode::LOGOUT, "logout", {}, "Log out of the current game."},
        {CommandCode::GET_SHIP_TYPES, "types", {}, "List available ship types."},
        {CommandCode::PLACE_SHIP, "place", {"<type...>", "<x>", "<y>", "(x|y)"}, "Place a ship."},
        {CommandCode::GET_MAP, "map", {"[own]"}, "Show opponent's or own map."},
        {CommandCode::GET_STATUS, "status", {}, "Show game status."},
        {CommandCode::MAKE_TURN, "fire", {"<x>", "<y>"}, "Fire a shot."},
    });

    class CLIClient : GameClient {
    private:
        using GameClient::listGames;
        using GameClient::newGame;
        using GameClient::join;
        using GameClient::logout;
        using GameClient::getShipTypes;
        using GameClient::getMap;
        using GameClient::placeShip;
        using GameClient::getStatus;
        using GameClient::fireAt;

        static void help(Command const &command, std::vector<std::string> const &args);
        void listGames(Command const &command, std::vector<std::string> const &args);
        void newGame(Command const &command, std::vector<std::string> const &args);
        void joinGame(Command const &command, std::vector<std::string> const &args);
        void logout(Command const &command, std::vector<std::string> const &args);
        void getShipTypes(Command const &command, std::vector<std::string> const &args);
        void placeShip(Command const &command, std::vector<std::string> const &args);
        void getMap(Command const &command, std::vector<std::string> const &args);
        void makeTurn(Command const &command, std::vector<std::string> const &args);
        void getStatus(Command const &command, std::vector<std::string> const &args);
        void handleCommand(std::string const &command, std::vector<std::string>const &args);
        void handleProtocolError(ProtocolError const &error);
        void handleCommandSafely(std::string const &command, std::vector<std::string>const &args);

        static Command getCommand(std::string const &command);
    public:
        using GameClient::GameClient;

        void loop();
    };
}