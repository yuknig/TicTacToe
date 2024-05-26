cmake_minimum_required(VERSION 3.5)
project (TicTacToe)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /permissive-")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TicTacToe)
endif()

include_directories(src/.)
add_subdirectory(src/gamelib)
add_subdirectory(src/app)
