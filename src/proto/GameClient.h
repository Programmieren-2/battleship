//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMECLIENT_H
#define BATTLESHIP_GAMECLIENT_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "BasicShip.h"
#include "Coordinate.h"
#include "Net.h"
#include "Client.h"
#include "Messages.h"

namespace proto {
    class GameClient : public net::Client {
    private:
        using net::Client::communicate;

        unsigned long gameId;
        unsigned long playerId;
        bool gameOver;
        bool won;

        template <typename RequestType>
        std::string sendMessage(RequestType const &request)
        {
            std::string buf = communicate(serialize(request));
            auto header = deserialize<ResponseHeader>(buf, true);
            if (header.type == ResponseType::INVALID_REQUEST)
                throw deserialize<InvalidRequest>(buf, true);

            return buf;
        }

        template <typename RequestType, typename ResponseType>
        ResponseType exchangeMessage(RequestType const &request)
        {
            return deserialize<ResponseType>(sendMessage(request));
        }
    public:
        GameClient(std::string const &host, unsigned short port);
        GameClient();

        [[nodiscard]] unsigned long getGameId() const;
        [[nodiscard]] unsigned long getPlayerId() const;
        void setGameId(unsigned long newGameId);
        void setPlayerId(unsigned long newPlayerId);

        std::vector<ListedGame> listGames();
        unsigned long newGame(unsigned short width, unsigned short height);
        bool join(unsigned long gameId, std::string const &name);
        bool logout();
        models::ShipTypes getShipTypes();
        std::string getMap(bool own = false);
        models::PlacementResult placeShip(models::BasicShip const &ship);
        GameState getStatus();
        models::HitResult fireAt(models::Coordinate const &target);

        bool isLoggedIn() const;
        void teardown();
    };
}

#endif //BATTLESHIP_GAMECLIENT_H
