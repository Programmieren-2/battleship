#include "Server.h"
using net::Server;

int main()
{
    Server server;
    server.listen();
    return 0;
}