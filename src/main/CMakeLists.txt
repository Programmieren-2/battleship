add_executable(battleship main.cpp)
target_link_libraries(battleship models)
target_include_directories(battleship PUBLIC ../models)