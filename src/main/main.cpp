#include <iostream>
using std::cout;
using std::endl;

#include "Coordinate.h"
using models::Coordinate;
using models::Orientation;

#include "GameBoard.h"
using models::GameBoard;

#include "PlayerBoard.h"
using models::PlayerBoard;
using models::PlayerBoards;

#include "Ship.h"
using models::Ship;
using models::Ships;

#include "ShipTypes.h"
using models::SpeedBoat;
using models::Destroyer;
using models::Submarine;
using models::Battleship;
using models::AircraftCarrier;

static Ships getPlayerShips()
{
    Ships ships;
    ships.push_back(SpeedBoat(Coordinate(1, 2), Orientation::X));
    ships.push_back(Destroyer(Coordinate(3, 5), Orientation::Y));
    ships.push_back(Submarine(Coordinate(9, 5), Orientation::Y));
    ships.push_back(Battleship(Coordinate(6, 9), Orientation::Y));
    ships.push_back(AircraftCarrier(Coordinate(3, 3), Orientation::X));
    return ships;
}

static bool checkShips(Ships const &ships)
{
    for (Ship ship : ships) {
        for (Ship other: ships) {
            if (ship != other && ship.collidesWith(other)) {
                cout << "Ship " << ship.toString() << " collides with " << other.toString() << endl;
                return false;
            }
        }
    }

    return true;
}

int main()
{
    Ships player1ships = getPlayerShips();
    Ships player2ships = getPlayerShips();
    GameBoard gameBoard;
    PlayerBoards playerBoards = gameBoard.getPlayerBoards();
    PlayerBoard player1board = playerBoards[0];
    PlayerBoard player2board = playerBoards[1];

    cout << "Player 1 board: " << player1board.getWidth() << "x" << player1board.getHeight() << endl;
    cout << "Player 2 board: " << player2board.getWidth() << "x" << player2board.getHeight() << endl;

    for (Ship ship : player1ships) {
        if (!player1board.placeShip(ship))
            cout << "Cannot place ship: " + ship.toString() << "for player 1" << endl;
    }

    for (Ship ship : player2ships) {
        if (!player2board.placeShip(ship))
            cout << "Cannot place ship: " + ship.toString() << "for player 2" << endl;
    }

    Coordinate target = Coordinate(6, 8);
}