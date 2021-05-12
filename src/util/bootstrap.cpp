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

#include "Constants.h"

#include "Coordinate.h"
using models::Coordinate;
using models::Orientation;

#include "PlayerBoard.h"
using models::PlacementResult;
using models::PlayerBoard;

#include "Ship.h"
using models::Ship;
using models::Ships;

#include "util.h"
using util::isNumber;
using util::readWithPrompt;
using util::splitString;

#include "bootstrap.h"

namespace bootstrap {
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

    static void readShip(PlayerBoard &playerBoard, string type, unsigned short length) {
        Coordinate anchorPoint = readCoordinate();
        Orientation orientation = readOrientation();

        Ship ship(type, anchorPoint, length, orientation);

        switch (playerBoard.placeShip(ship)) {
            case PlacementResult::NOT_ON_BOARD:
                cerr << "Ship is not on the board.\n";
                readShip(playerBoard, type, length);
                return;
            case PlacementResult::COLLISION:
                cerr << "Ship collides with another ship.\n";
                readShip(playerBoard, type, length);
                return;
            case PlacementResult::SUCCESS:
                return;
        }
    }

    static void readShips(PlayerBoard &playerBoard) {
        for (auto const &[type, length] : models::Constants::SHIP_TYPES) {
            cout << "Place your " << type << " (size " << length << ").\n";
            readShip(playerBoard, type, length);
            cout << playerBoard.toString(true);
        }
    }

    PlayerBoard readPlayerBoard() {
        static unsigned short playerNum = 0;
        playerNum++;
        string name = readWithPrompt("Enter name of player #" + to_string(playerNum) + ": ");
        PlayerBoard playerBoard(name);
        cout << playerBoard.toString(true);
        readShips(playerBoard);
        return playerBoard;
    }
}