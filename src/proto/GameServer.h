//
// Created by rne on 08.05.21.
//

#ifndef BATTLESHIP_GAMESERVER_H
#define BATTLESHIP_GAMESERVER_H

#include <optional>
#include <string>

#include "Ship.h"

#include "Net.h"

#include "Messages.h"
#include "Player.h"
#include "Server.h"

namespace proto {
    class GameServer : public net::Server {
    private:
        Players players;
        models::ShipTypes shipTypes;
        GameState state;

        [[nodiscard]] std::optional<Player> getPlayer(unsigned long playerId) const;
        [[nodiscard]] std::optional<Player> getOpponent(unsigned long playerId) const;
        [[nodiscard]] std::string processLoginRequest(LoginRequest const &request);
        [[nodiscard]] std::string processShipTypesRequest(ShipTypesRequest const &request) const;
        [[nodiscard]] std::string processMapRequest (MapRequest const &request) const;
        [[nodiscard]] std::string processShipPlacementRequest(ShipPlacementRequest const &request) const;
        [[nodiscard]] std::string processStatusRequest(StatusRequest const &request) const;
        [[nodiscard]] std::string processTurnRequest(TurnRequest const &request) const;
    protected:
        [[nodiscard]] std::string handleRequest(std::string const &buf) override;
    public:
        GameServer(string const &host, unsigned short port, models::ShipTypes shipTypes);
        GameServer(string const &host, unsigned short port);
        explicit GameServer(models::ShipTypes shipTypes);
        GameServer();
    };
}

#endif //BATTLESHIP_GAMESERVER_H
