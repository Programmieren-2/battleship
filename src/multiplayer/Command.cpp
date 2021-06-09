//
// Created by rne on 09.06.21.
//

#include <iostream>
using std::ostream;

#include <map>
using std::map;

#include <string>
using std::string;

#include <utility>
using std::move;

#include <vector>
using std::vector;

#include "util.h"
using util::joinStrings;

#include "Command.h"

namespace multiplayer {
    Command::Command(CommandCode id, string name, vector<string> const &args, string help)
        : id(id), name(move(name)), args(args), help(move(help))
    {}

    CommandCode Command::getId() const
    {
        return id;
    }

    string Command::getName() const
    {
        return name;
    }

    string Command::usage() const
    {
        string usage = help + "\nUsage: " + name;

        if (!args.empty())
            usage += " " + joinStrings(args);

        return usage + "\n";
    }

    string Command::operator+(string const &other)
    {
        return other + name;
    }

    ostream& operator<<(ostream& out, Command const &command)
    {
        out << command.usage();
        return out;
    }

    string operator+(string const &other, Command const &command)
    {
        return other + command.name;
    }

    Command Command::invalid()
    {
        return Command(INVALID, "", {}, "");
    }

    map<string, Command> make_map(vector<Command> const &commands)
    {
        map<string, Command> result;

        for (auto const &command : commands)
            result.insert({command.getName(), command});

        return result;
    }
}