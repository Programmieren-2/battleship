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
        string input;
        vector<string> items;
        string strX, strY;
        unsigned short x, y;

        while (true) {
            input = readWithPrompt(prompt);
            items = splitString(input, ",");

            if (items.size() == 2) {
                strX = items[0];
                strY = items[1];

                if (isNumber(strX) && isNumber(strY)) {
                    x = static_cast<unsigned short>(stoul(strX));
                    y = static_cast<unsigned short>(stoul(strY));
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
        Coordinate anchorPoint = readCoordinate();
        Orientation orientation = readOrientation();

        Ship ship(type, anchorPoint, length, orientation);

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