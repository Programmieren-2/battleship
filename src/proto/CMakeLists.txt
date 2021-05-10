add_library(proto GameClient.cpp GameClient.h GameServer.cpp GameServer.h Packets.h)
target_include_directories(proto PUBLIC ../models ../net ../util)
target_link_libraries(proto net models util)