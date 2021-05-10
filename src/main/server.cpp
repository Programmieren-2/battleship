#include <map>
using std::map;

#include <string>
using std::string;

#include "Constants.h"

#include "GameServer.h"
using proto::GameServer;

int main()
{
    map<string, unsigned short> shipTypes = models::Constants::shipTypes;
    shipTypes["Fishing Boat"] = 1;
    GameServer server(shipTypes);
    server.listen();
    return 0;
}