add_library(proto
        BufferSizeMismatch.cpp BufferSizeMismatch.h
        GameClient.cpp GameClient.h
        GameServer.cpp GameServer.h
        Messages.h
        OnlinePlayer.cpp OnlinePlayer.h
        OnlineGame.cpp OnlineGame.h
)
target_include_directories(proto PUBLIC ../models ../net ../util)
target_link_libraries(proto net models util)