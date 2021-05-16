add_executable(bssrv server.cpp)
target_include_directories(bssrv PUBLIC ../models ../proto ../util)
target_link_libraries(bssrv models proto util)

add_executable(bsclt client.cpp)
target_include_directories(bsclt PUBLIC ../models ../proto ../util)
target_link_libraries(bsclt models proto util)