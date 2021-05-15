//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMESERVER_H
#define BATTLESHIP_GAMESERVER_H

#include <optional>
#include <string>
#include <vector>

#include "Game.h"
#include "Ship.h"

#include "Server.h"

#include "Messages.h"
#include "OnlineGame.h"

namespace proto {
    typedef std::vector<OnlineGame> Games;

    class GameServer : public net::Server {
    private:
        static unsigned long gameId;
        Games games;

        [[nodiscard]] unsigned long addGame(unsigned short width, unsigned short height);
        [[nodiscard]] std::string processNewGameRequest(NewGameRequest const &request);
        [[nodiscard]] std::string handleRequest(std::string const &buf) override;
    public:
        GameServer(string const &host, unsigned short port);
        GameServer();

        std::optional<OnlineGame> getGame(unsigned long id);
    };
}

#endif //BATTLESHIP_GAMESERVER_H
