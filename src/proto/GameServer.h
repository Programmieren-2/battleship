//
// Created by rne on 08.05.21.
//

#pragma once

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "Game.h"
#include "Ship.h"

#include "Server.h"

#include "Messages.h"
#include "OnlineGame.h"

#include "boostwrap.h"

namespace proto {
    using Games = std::vector<OnlineGame>;

    class GameServer : public net::Server {
    private:
        static unsigned long gameId;
        Games games;

        [[nodiscard]] std::optional<std::reference_wrapper<OnlineGame>> getGame(unsigned long id);
        [[nodiscard]] unsigned long addGame(unsigned short width, unsigned short height);

        [[nodiscard]] NewGameResponse processNewGameRequest(NewGameRequest const &request);
        [[nodiscard]] std::string processNewGameRequest(std::string const &buf);

        [[nodiscard]] std::string processListGamesRequest() const;
        [[nodiscard]] std::string handleRequest(std::string const &buf) override;
    public:
        GameServer(boost::asio::ip::address ipAddress, unsigned short port);
    };
}