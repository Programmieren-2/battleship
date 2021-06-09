add_library(util ip.cpp ip.h util.cpp util.h util.inl)
target_link_libraries(util ${Boost_LIBRARIES})
target_include_directories(util PUBLIC ../os)