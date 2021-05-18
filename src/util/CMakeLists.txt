add_library(util bootstrap.cpp bootstrap.h util.cpp util.h)
target_link_libraries(util models ${Boost_LIBRARIES})
target_include_directories(util PUBLIC ../models)