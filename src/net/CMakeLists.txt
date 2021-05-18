add_library(net Client.cpp Client.h Net.h Server.cpp Server.h SocketError.cpp SocketError.h TCPService.cpp TCPService.h)
target_include_directories(net PRIVATE ../base64)

if(MSVC)
    target_link_libraries(net base64 ${Boost_LIBRARIES})
else()
    target_link_libraries(net base64)
endif()