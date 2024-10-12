cmake_minimum_required(VERSION 3.13)

set(PRJ_NAME 01_lca)
project(${PRJ_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# OneTBB
include_directories(../utils/)
file(GLOB SRC "*.h" "*.hpp" "*.cpp")

add_executable(${PRJ_NAME} ${SRC})