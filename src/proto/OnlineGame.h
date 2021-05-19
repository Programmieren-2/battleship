//
// Created by rne on 15.05.21.
//

#ifndef BATTLESHIP_ONLINEGAME_H
#define BATTLESHIP_ONLINEGAME_H

#include <optional>
#include <string>

#include "Game.h"
#include "Sea.h"

#include "Messages.h"
#include "OnlinePlayer.h"

namespace proto {
    class OnlineGame : public models::Game<OnlinePlayer> {
    private:
        using models::Game<OnlinePlayer>::getShipTypes;
        using models::Game<OnlinePlayer>::getPlayers;
        using models::Game<OnlinePlayer>::addPlayer;

        unsigned long id;
        GameState state;
        std::optional<OnlinePlayer> currentPlayer;

        [[nodiscard]] models::Sea makeSea() const;
        [[nodiscard]] bool allShipsPlaced(models::Sea const &sea);

        [[nodiscard]] LoginResponse processLoginRequest(LoginRequest const &request);
        [[nodiscard]] LogoutResponse processLogoutRequest(LogoutRequest const &request);
        [[nodiscard]] std::string processShipTypesRequest(ShipTypesRequest const &request);
        [[nodiscard]] std::string processMapRequest(MapRequest const &request);
        [[nodiscard]] ShipPlacementResponse processShipPlacementRequest(ShipPlacementRequest const &request);
        [[nodiscard]] StatusResponse processStatusRequest(StatusRequest const &request);
        [[nodiscard]] TurnResponse processTurnRequest(TurnRequest const &request);
    public:
        using models::Game<OnlinePlayer>::getWidth;
        using models::Game<OnlinePlayer>::getHeight;
        using models::Game<OnlinePlayer>::getPlayerCount;

        OnlineGame(unsigned long id, unsigned short width, unsigned short height);

        [[nodiscard]] unsigned long getId() const;
        [[nodiscard]] auto getOpponent(unsigned long playerId);
        [[nodiscard]] auto getPlayer(unsigned long playerId);

        [[nodiscard]] std::string processLoginRequest(std::string const &buf);
        [[nodiscard]] std::string processLogoutRequest(std::string const &buf);
        [[nodiscard]] std::string processShipTypesRequest(std::string const &buf);
        [[nodiscard]] std::string processMapRequest(std::string const &buf);
        [[nodiscard]] std::string processShipPlacementRequest(std::string const &buf);
        [[nodiscard]] std::string processStatusRequest(std::string const &buf);
        [[nodiscard]] std::string processTurnRequest(std::string const &buf);
    };
}

#endif //BATTLESHIP_ONLINEGAME_H
