add_executable(singleplayer bootstrap.cpp bootstrap.h main.cpp game.cpp game.h)
target_link_libraries(singleplayer models util)
target_include_directories(singleplayer PUBLIC ../models ../util)