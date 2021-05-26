add_executable(bssrv server.cpp)
target_include_directories(bssrv PUBLIC ../proto ../util)
target_link_libraries(bssrv proto util ${Boost_LIBRARIES})

add_executable(bsclt client.cpp defaults.h)
target_include_directories(bsclt PUBLIC ../proto ../util)
target_link_libraries(bsclt proto util ${Boost_LIBRARIES})