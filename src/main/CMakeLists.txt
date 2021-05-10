add_executable(battleship main.cpp game.cpp game.h)
target_link_libraries(battleship models util)
target_include_directories(battleship PUBLIC ../models ../util)

add_executable(server server.cpp)
target_include_directories(server PUBLIC ../models ../proto ../util)
target_link_libraries(server models proto util)

add_executable(client client.cpp)
target_include_directories(client PUBLIC ../models ../proto ../util)
target_link_libraries(client models proto util)