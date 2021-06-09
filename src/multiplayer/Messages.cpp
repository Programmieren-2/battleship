//
// Created by rne on 09.06.21.
//

#include <iostream>
using std::ostream;

#include "Messages.h"

namespace multiplayer {
    ostream& operator<<(ostream &out, ListedGame const &game)
    {
        out << "Game #" << game.id << ":\n";
        out << "  Sea size: " << static_cast<unsigned short>(game.width)
            << "x" << static_cast<unsigned short>(game.height) << "\n";
        out << "  Current players: " << static_cast<unsigned long>(game.players) << "\n";
        return out;
    }
}