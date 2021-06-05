//
// Created by rne on 08.05.21.
//

#include <string>
using std::string;

#include "Client.h"

namespace tcp {
    string Client::communicate(string const &bytes) const
    {
        socket.connect(getEndpoint());
        send(bytes);
        string response = receive();
        socket.close();
        return response;
    }
}
