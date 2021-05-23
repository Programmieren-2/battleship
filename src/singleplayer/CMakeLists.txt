add_executable(battleship engine.cpp engine.h main.cpp)
target_link_libraries(battleship models util)
target_include_directories(battleship PUBLIC ../models ../util)