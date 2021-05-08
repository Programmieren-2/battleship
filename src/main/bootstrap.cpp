//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cout;
using std::endl;

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
using util::splitString;
using util::readWithPrompt;

#include "bootstrap.h"

namespace bootstrap {
    Coordinate readCoordinate(string const &prompt) {
        string input = readWithPrompt(prompt);
        vector<string> items = splitString(input, ",");

        if (items.size() != 2) {
            cout << "Invalid coordinates!" << endl;
            return readCoordinate(prompt);
        }

        unsigned short x = stoi(items[0]);
        unsigned short y = stoi(items[1]);
        return Coordinate(x, y);
    }

    Orientation readOrientation(std::string const &prompt) {
        string input = readWithPrompt(prompt);

        if (input == "x" || input == "X")
            return Orientation::X;

        if (input == "y" || input == "Y")
            return Orientation::Y;

        cout << "Invalid orientation!" << endl;
        return readOrientation(prompt);
    }

    void readShip(PlayerBoard &playerBoard, string type, unsigned short length) {
        Coordinate anchorPoint = readCoordinate("Coordinates (<x>,<y>): ");
        Orientation orientation = readOrientation("Orientation (x or y): ");
        Ship ship(type, anchorPoint, length, orientation);

        switch (playerBoard.placeShip(ship)) {
            case PlacementResult::NOT_ON_BOARD:
                cout << "Ship is not on the board." << endl;
                readShip(playerBoard, type, length);
                return;
            case PlacementResult::COLLISION:
                cout << "Ship collides with another ship." << endl;
                readShip(playerBoard, type, length);
                return;
            case PlacementResult::SUCCESS:
                return;
        }

        cout << "Something went awry. Try again." << endl;
        readShip(playerBoard, type, length);
    }

    void readShips(PlayerBoard &playerBoard) {
        for (auto const &[type, length] : models::Constants::shipTypes) {
            cout << "Place your " << type << "." << endl;
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