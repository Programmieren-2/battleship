//
// Created by rne on 15.05.21.
//

#pragma once

#include <string>
#include <vector>

#include "GameClient.h"
#include "ProtocolError.h"

namespace proto {
    enum Command {
        INVALID,
        HELP,
        LIST_GAMES,
        NEW_GAME,
        JOIN,
        LOGOUT,
        GET_SHIP_TYPES,
        PLACE_SHIP,
        GET_MAP,
        GET_STATUS,
        MAKE_TURN,
    };

    const std::map<std::string, Command> COMMANDS = {
            {"help", Command::HELP},
            {"list", Command::LIST_GAMES},
            {"new", Command::NEW_GAME},
            {"join", Command::JOIN},
            {"logout", Command::LOGOUT},
            {"types", Command::GET_SHIP_TYPES},
            {"place", Command::PLACE_SHIP},
            {"map", Command::GET_MAP},
            {"status", Command::GET_STATUS},
            {"fire", Command::MAKE_TURN},
    };

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

        static void help(std::string const &string, std::vector<std::string> const &args);
        void listGames(std::string const &string, std::vector<std::string> const &args);
        void newGame(std::string const &command, std::vector<std::string> const &args);
        void joinGame(std::string const &command, std::vector<std::string> const &args);
        void logout(std::string const &string, std::vector<std::string> const &args);
        void getShipTypes(std::string const &command, std::vector<std::string> const &args);
        void placeShip(std::string const &command, std::vector<std::string> const &args);
        void getMap(std::string const &command, std::vector<std::string> const &args);
        void makeTurn(std::string const &command, std::vector<std::string> const &args);
        void getStatus(std::string const &command, std::vector<std::string> const &args);
        void handleCommand(std::string const &command, std::vector<std::string>const &args);
        void handleProtocolError(ProtocolError const &error);
        void handleCommandSafely(std::string const &command, std::vector<std::string>const &args);

        static Command getCommand(std::string const &command);
    public:
        using GameClient::GameClient;

        void loop();
    };
}