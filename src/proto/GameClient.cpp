//
// Created by rne on 08.05.21.
//

#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include "Client.h"
using net::Client;

#include "Net.h"
using net::Bytes;
using net::Socket;

#include "Packets.h"

#include "GameClient.h"

namespace proto {
    Bytes GameClient::receive(Socket &socket)
    {
        Bytes buf = receive(socket, sizeof(ResponseHeader));
        ResponseHeader header;
        memcpy(&header, &buf[0], buf.size());
        cerr << "Received response " << header.type << "." << endl;

        switch (header.type) {
            case ResponseType::LOGIN_RESPONSE:
                cerr << "Got valid login response." << endl;
                return processLoginResponse(readPacket<LoginResponse>(socket, buf));
        }
    }

    Bytes GameClient::processLoginResponse(LoginResponse const &loginResponse)
    {
        LoginResponse response = loginResponse;
        return packetToBytes(response);
    }

    bool GameClient::sendLoginRequest(string const &name)
    {
        LoginRequest loginRequest;

        if (name.size() > 32) {
            cerr << "Name too long." << endl;
            return false;
        }

        loginRequest.header.playerId = 42;
        memcpy(&loginRequest.playerName, &name, name.length());
        Bytes response = communicate(packetToBytes(loginRequest));
        return true;
    }
}