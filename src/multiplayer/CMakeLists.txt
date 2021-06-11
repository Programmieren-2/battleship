add_library(multiplayer
        CLIClient.cpp CLIClient.h
        CLIError.cpp CLIError.h
        Command.cpp Command.h
        Defaults.h
        GameClient.cpp GameClient.h GameClient.inl
        GameServer.cpp GameServer.h
        Messages.cpp Messages.h Messages.inl
        OnlineGame.cpp OnlineGame.h
        OnlinePlayer.cpp OnlinePlayer.h
        ProtocolError.cpp ProtocolError.h
)
target_include_directories(multiplayer PUBLIC ../models ../tcp ../os ../util)
target_link_libraries(multiplayer tcp models util)

add_executable(bssrv server.cpp)
target_include_directories(bssrv PUBLIC ../util)
target_link_libraries(bssrv multiplayer util ${Boost_LIBRARIES})

add_executable(bsclt client.cpp Defaults.h)
target_include_directories(bsclt PUBLIC ../util)
target_link_libraries(bsclt multiplayer util ${Boost_LIBRARIES})