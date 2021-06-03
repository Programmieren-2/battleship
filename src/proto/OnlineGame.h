//
// Created by rne on 15.05.21.
//

#pragma once

#include <functional>
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
        mutable GameState state;
        mutable std::optional<std::reference_wrapper<const OnlinePlayer>> currentPlayer;

        [[nodiscard]] models::Sea makeSea() const;
        [[nodiscard]] bool allPlayersOnline() const;
        [[nodiscard]] bool allShipsPlaced(models::Sea const &sea) const;
        [[nodiscard]] bool allPlayersReady() const;
        [[nodiscard]] models::PlacementResult placeShip(ShipPlacementRequest const &request, models::Sea &sea) const;

        [[nodiscard]] LoginResponse processLoginRequest(LoginRequest const &request) const;
        [[nodiscard]] LogoutResponse processLogoutRequest(LogoutRequest const &request) const;
        [[nodiscard]] std::string processShipTypesRequest(ShipTypesRequest const &request) const;
        [[nodiscard]] std::string processMapRequest(MapRequest const &request) const;
        [[nodiscard]] ShipPlacementResponse processShipPlacementRequest(ShipPlacementRequest const &request) const;
        [[nodiscard]] StatusResponse processStatusRequest(StatusRequest const &request) const;
        [[nodiscard]] TurnResponse processTurnRequest(TurnRequest const &request) const;
    public:
        using models::Game<OnlinePlayer>::getWidth;
        using models::Game<OnlinePlayer>::getHeight;
        using models::Game<OnlinePlayer>::getPlayerCount;

        OnlineGame(unsigned long id, unsigned short width, unsigned short height);

        [[nodiscard]] unsigned long getId() const;
        [[nodiscard]] GameState getState() const;
        [[nodiscard]] std::optional<std::reference_wrapper<const OnlinePlayer>> getOpponent(unsigned long playerId) const;
        [[nodiscard]] std::optional<std::reference_wrapper<const OnlinePlayer>> getPlayer(unsigned long playerId) const;

        [[nodiscard]] std::string processLoginRequest(std::string const &buf) const;
        [[nodiscard]] std::string processLogoutRequest(std::string const &buf) const;
        [[nodiscard]] std::string processShipTypesRequest(std::string const &buf) const;
        [[nodiscard]] std::string processMapRequest(std::string const &buf) const;
        [[nodiscard]] std::string processShipPlacementRequest(std::string const &buf) const;
        [[nodiscard]] std::string processStatusRequest(std::string const &buf) const;
        [[nodiscard]] std::string processTurnRequest(std::string const &buf) const;

        [[nodiscard]] bool operator==(OnlineGame const &other) const;
        [[nodiscard]] bool operator<(OnlineGame const &other) const;
    };
}