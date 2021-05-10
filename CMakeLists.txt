cmake_minimum_required(VERSION 3.18)
project(battleship)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/W4)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost 1.76.0 REQUIRED COMPONENTS date_time)
    if (Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
    endif()
else()
    add_compile_options(-Wall -Wextra -pedantic)
    set(CMAKE_CXX_FLAGS -pthread)
endif()

add_subdirectory(src)