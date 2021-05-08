add_executable(battleship main.cpp util.cpp util.h bootstrap.cpp bootstrap.h game.cpp game.h)
target_link_libraries(battleship models)
target_include_directories(battleship PUBLIC ../models)

add_executable(server server.cpp)
target_link_libraries(server models net)
target_include_directories(server PUBLIC ../models ../net)

add_executable(client client.cpp)
target_link_libraries(client models net)
target_include_directories(client PUBLIC ../models ../net)