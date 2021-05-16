add_executable(battleship main.cpp game.cpp game.h)
target_link_libraries(battleship models util)
target_include_directories(battleship PUBLIC ../models ../util)