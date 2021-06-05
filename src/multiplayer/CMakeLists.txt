add_library(multiplayer
        ../multiplayer/CLIClient.cpp ../multiplayer/CLIClient.h
        ../multiplayer/GameClient.cpp ../multiplayer/GameClient.h
        ../multiplayer/GameServer.cpp ../multiplayer/GameServer.h
        ../multiplayer/Messages.h
        ../multiplayer/OnlineGame.cpp ../multiplayer/OnlineGame.h
        ../multiplayer/OnlinePlayer.cpp ../multiplayer/OnlinePlayer.h
        ../multiplayer/ProtocolError.cpp ../multiplayer/ProtocolError.h)
target_include_directories(multiplayer PUBLIC ../models ../tcp ../os ../util)
target_link_libraries(multiplayer net models util)

add_executable(bssrv server.cpp)
target_include_directories(bssrv PUBLIC ../util)
target_link_libraries(bssrv multiplayer util ${Boost_LIBRARIES})

add_executable(bsclt client.cpp defaults.h)
target_include_directories(bsclt PUBLIC ../util)
target_link_libraries(bsclt multiplayer util ${Boost_LIBRARIES})