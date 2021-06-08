add_library(tcp Client.cpp Client.h Server.cpp Server.h SocketError.cpp SocketError.h TCPService.cpp TCPService.h)
target_include_directories(tcp PRIVATE ../base64 ../os)

if(MSVC)
    target_link_libraries(tcp base64 ${Boost_LIBRARIES})
else()
    target_link_libraries(tcp base64)
endif()