add_library(net Client.cpp Client.h Net.h Server.cpp Server.h TCPIO.cpp TCPIO.h)
target_link_libraries(net base64)
target_include_directories(net PRIVATE ../base64)