add_library(proto Messages.h)
target_link_libraries(proto net models)
target_include_directories(proto PUBLIC ../models ../net)