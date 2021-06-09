//
// Created by rne on 09.06.21.
//

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace multiplayer {
    enum CommandCode {
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

    class Command {
    private:
        CommandCode id;
        std::string name;
        std::vector<std::string> args;
        std::string help;
    public:
        Command(CommandCode id, std::string name, std::vector<std::string> const &args, std::string help);

        [[nodiscard]] CommandCode getId() const;
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string usage() const;

        std::string operator+(std::string const &other);
        friend std::ostream& operator<<(std::ostream& out, Command const &command);
        friend std::string operator+(std::string const &other, Command const &command);

        static Command invalid();
    };

    std::map<std::string, Command> make_map(std::vector<Command> const &command);
}