add_executable(battleship main.cpp util.cpp util.h bootstrap.cpp bootstrap.h game.cpp game.h)
target_link_libraries(battleship models)
target_include_directories(battleship PUBLIC ../models)