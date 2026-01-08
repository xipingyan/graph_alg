cmake_minimum_required(VERSION 3.13)

set(PRJ_NAME 03_dag_asyn_test)
project(${PRJ_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# OneTBB
include_directories(../utils/)
file(GLOB SRC "*.h" "*.hpp" "*.cpp" "../utils/*.cpp" "../utils/*.hpp")

# Group utility files in a folder in Solution Explorer
source_group("utils" REGULAR_EXPRESSION ".*/utils/.*")

add_executable(${PRJ_NAME} ${SRC})