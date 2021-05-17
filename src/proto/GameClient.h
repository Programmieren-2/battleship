//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMECLIENT_H
#define BATTLESHIP_GAMECLIENT_H

#include <map>
#include <string>

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

        template <typename RequestType>
        std::string communicate(RequestType const &request)
        {
            std::string buf = communicate(serialize(request));
            auto header = deserialize<ResponseHeader>(buf, true);
            if (header.type == ResponseType::INVALID_REQUEST)
                throw deserialize<InvalidRequest>(buf, true);

            return buf;
        }

        template <typename RequestType, typename ResponseType>
        ResponseType communicate(RequestType const &request)
        {
            return deserialize<ResponseType>(communicate(request));
        }
    public:
        GameClient(std::string const &host, unsigned short port);
        GameClient();

        [[nodiscard]] unsigned long getGameId() const;
        [[nodiscard]] unsigned long getPlayerId() const;
        void setGameId(unsigned long newGameId);
        void setPlayerId(unsigned long newPlayerId);

        bool login(std::string const &name);
        models::ShipTypes getShipTypes();
        std::string getMap(bool own = false);
        PlacementResult placeShip(models::BasicShip const &ship);
        GameState getStatus();
        std::vector<ListedGame> listGames();
        unsigned long newGame(unsigned short width, unsigned short height);
        bool join(unsigned long gameId, std::string const &name);
    };
}

#endif //BATTLESHIP_GAMECLIENT_H
