//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Ship.h"
using models::ShipTypes;

#include "Client.h"
using net::Client;

#include "util.h"
using util::copyString;

#include "Messages.h"
#include "GameClient.h"

namespace proto {
    GameClient::GameClient(string const &host, unsigned short port)
            : Client(host, port), playerId(0)
    {}

    GameClient::GameClient()
            : Client(), playerId(0)
    {}

    bool GameClient::login(string const &name)
    {
        LoginRequest loginRequest;
        copyString(loginRequest.playerName, name, sizeof loginRequest.playerName);
        LoginResponse response = communicate<LoginRequest, LoginResponse>(loginRequest);
        playerId = response.header.playerId;
        return response.accepted;
    }

    ShipTypes GameClient::getShipTypes()
    {
        ShipTypesRequest request;
        request.header.playerId = playerId;
        string buf = communicate(serialize(request));
        auto response = deserialize<ShipTypesResponse>(buf, true);
        ShipTypes shipTypes;
        ShipType shipType;
        size_t offset;

        for (unsigned short i = 0; i < response.ships; i++) {
            offset = i * sizeof shipType + sizeof response;
            shipType = deserialize<ShipType>(buf.substr(offset, sizeof shipType), true);
            shipTypes[shipType.name] = shipType.size;
        }

        return shipTypes;
    }

    string GameClient::getMap()
    {
        MapRequest request;
        request.header.playerId = playerId;
        string buf = communicate(serialize(request));
        auto header = deserialize<ResponseHeader>(buf, true);

        if (header.type == ResponseType::INVALID_REQUEST)
            return "Invalid request.\n";

        auto response = deserialize<MapResponse>(buf, true);
        return buf.substr(sizeof response, response.size);
    }
}