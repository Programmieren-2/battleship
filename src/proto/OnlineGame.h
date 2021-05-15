//
// Created by rne on 15.05.21.
//

#ifndef BATTLESHIP_ONLINEGAME_H
#define BATTLESHIP_ONLINEGAME_H

#include <functional>
#include <string>

#include "Game.h"
#include "Sea.h"

#include "Messages.h"
#include "OnlinePlayer.h"

namespace proto {
    class OnlineGame : models::Game<OnlinePlayer> {
    private:
        unsigned long id;

        models::Sea makeSea() const;
    public:
        using models::Game<OnlinePlayer>::getWidth;
        using models::Game<OnlinePlayer>::getHeight;
        using models::Game<OnlinePlayer>::getShipTypes;
        using models::Game<OnlinePlayer>::getPlayers;
        using models::Game<OnlinePlayer>::addPlayer;

        OnlineGame(unsigned long id, unsigned short width, unsigned short height);

        unsigned long getId() const;

        auto getOpponent(unsigned long playerId);
        auto getPlayer(unsigned long playerId);

        std::string processLoginRequest(LoginRequest const &request);
        std::string processShipTypesRequest(ShipTypesRequest const &request) const;
        std::string processMapRequest(MapRequest const &request);
        std::string processShipPlacementRequest(ShipPlacementRequest const &request);
        std::string processStatusRequest(StatusRequest const &statusRequest) const;
        std::string processTurnRequest(TurnRequest const &turnRequest) const;
    };

    typedef std::reference_wrapper<OnlinePlayer> OnlineGameReference;
}

#endif //BATTLESHIP_ONLINEGAME_H
