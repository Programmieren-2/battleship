//
// Created by rne on 08.05.21.
//

#pragma once

#include <functional>
#include <optional>
#include <set>
#include <string>

#include "Game.h"
#include "Ship.h"

#include "Server.h"

#include "Messages.h"
#include "OnlineGame.h"

#include "boostwrap.h"

namespace proto {
    class GameServer : public tcp::Server {
    private:
        mutable std::set<OnlineGame> games;

        [[nodiscard]] std::optional<std::reference_wrapper<OnlineGame const>> getGame(unsigned long id) const;
        [[nodiscard]] unsigned long addGame(unsigned short width, unsigned short height) const;
        bool removeGame(unsigned long id) const;
        bool removeGame(OnlineGame const &game) const;
        void cleanup() const;

        [[nodiscard]] NewGameResponse processNewGameRequest(NewGameRequest const &request)const;
        [[nodiscard]] std::string processNewGameRequest(std::string const &buf) const;

        [[nodiscard]] std::string processListGamesRequest() const;
        [[nodiscard]] std::string handleRequest(std::string const &buf) const override;
    public:
        GameServer(boost::asio::ip::address const &ipAddress, unsigned short port);
    };
}