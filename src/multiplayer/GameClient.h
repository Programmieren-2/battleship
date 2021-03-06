//
// Created by rne on 08.05.21.
//

#pragma once

#include <string>
#include <vector>

#include "boostwrap.h"

#include "BasicShip.h"
#include "Coordinate.h"
#include "Client.h"
#include "Messages.h"
#include "ProtocolError.h"
#include "ShipTypes.h"

namespace multiplayer {
    class GameClient : public tcp::Client {
    private:
        using tcp::Client::communicate;

        unsigned long gameId;
        unsigned long playerId;
        bool gameOver;
        bool won;

        template <typename RequestType>
        std::string sendMessage(RequestType const &request) const;

        template <typename RequestType, typename ResponseType>
        ResponseType exchangeMessage(RequestType const &request) const;
    public:
        GameClient(boost::asio::ip::address const &ipAddress, unsigned short port);

        [[nodiscard]] unsigned long getGameId() const;
        [[nodiscard]] unsigned long getPlayerId() const;
        void setGameId(unsigned long newGameId);
        void setPlayerId(unsigned long newPlayerId);

        std::vector<ListedGame> listGames() const;
        unsigned long newGame(unsigned short width, unsigned short height);
        unsigned long newGame();
        bool join(unsigned long gameId, std::string const &name);
        bool logout();
        models::ShipTypes getShipTypes() const;
        std::string getMap(bool own = false) const;
        PlacementResult placeShip(models::BasicShip const &ship);
        GameState getStatus() const;
        models::HitResult fireAt(models::Coordinate const &target);

        [[nodiscard]] bool isLoggedIn() const;
        void teardown();
    };

#include "GameClient.inl"
}