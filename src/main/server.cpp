#include "GameServer.h"
using proto::GameServer;

int main()
{
    GameServer server;
    server.listen();
    return 0;
}