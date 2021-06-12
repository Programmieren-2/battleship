//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;
using std::cout;

#include <string>
using std::string;
using std::to_string;

#include <vector>
using std::vector;

#include "exceptions.h"
#include "Coordinate.h"
#include "Game.h"
#include "Player.h"
#include "Sea.h"
#include "Ship.h"

#include "util.h"
using util::isNumber;
using util::readWithPrompt;
using util::splitString;

#include "bootstrap.h"

namespace models {
    Coordinate readCoordinate(string const &prompt) {
        while (true) {
            auto items = splitString(readWithPrompt(prompt), ",");

            if (items.size() == 2) {
                if (isNumber(items[0]) && isNumber(items[1])) {
                    auto x = static_cast<unsigned short>(stoul(items[0]));
                    auto y = static_cast<unsigned short>(stoul(items[1]));
                    return Coordinate(x, y);
                }
            }

            cerr << "Invalid coordinates!\n";
        }
    }

    Coordinate readCoordinate()
    {
        return readCoordinate("Coordinates (<x>,<y>): ");
    }

    Orientation readOrientation(string const &prompt) {
        string input;

        while (true) {
            input = readWithPrompt(prompt);

            if (input == "x" || input == "X")
                return Orientation::X;

            if (input == "y" || input == "Y")
                return Orientation::Y;

            cerr << "Invalid orientation!\n";
        }
    }

    Orientation readOrientation()
    {
        return readOrientation("Orientation (x or y): ");
    }

    static void readShip(Sea &sea, string const &type, unsigned short length) {
        Ship ship(type, readCoordinate(), length, readOrientation());

        try {
            sea.placeShip(ship);
        } catch (PlaceShipError const &error) {
            cerr << error.what() << "\n";
        }
    }

    Sea readSea(unsigned short width, unsigned short height) {
        Sea sea(width, height);

        for (auto const &[type, length] : Game<Player>::SHIP_TYPES) {
            cout << sea.toString(true);
            cout << "Place your " << type << " (size " << length << ").\n";
            readShip(sea, type, length);
        }

        return sea;
    }
}