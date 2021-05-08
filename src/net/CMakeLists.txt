add_library(net Server.cpp Server.h Client.cpp Client.h RequestHandler.cpp RequestHandler.h Messages.cpp Messages.h)
target_link_libraries(net models)
target_include_directories(net PUBLIC ../models)