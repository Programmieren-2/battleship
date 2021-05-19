add_library(proto
        BufferSizeMismatch.cpp BufferSizeMismatch.h
        CLIClient.cpp CLIClient.h
        GameClient.cpp GameClient.h
        GameServer.cpp GameServer.h
        Messages.h
        OnlineGame.cpp OnlineGame.h
        OnlinePlayer.cpp OnlinePlayer.h
        ProtocolError.cpp ProtocolError.h)
target_include_directories(proto PUBLIC ../models ../net ../os ../util)
target_link_libraries(proto net models util)