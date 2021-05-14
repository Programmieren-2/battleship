add_executable(singleplayer main.cpp game.cpp game.h)
target_link_libraries(singleplayer models util)
target_include_directories(singleplayer PUBLIC ../models ../util)