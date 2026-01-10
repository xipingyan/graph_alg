cmake_minimum_required(VERSION 3.13)

set(PRJ_NAME 01_lca)
project(${PRJ_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OneTBB
find_package(TBB REQUIRED)
include_directories(../utils/)
file(GLOB SRC "*.h" "*.hpp" "*.cpp" "../utils/*.cpp" "../utils/*.hpp")

# Group utility files in a folder in Solution Explorer
source_group("utils" REGULAR_EXPRESSION ".*/utils/.*")

add_executable(${PRJ_NAME} ${SRC})
target_link_libraries(${PRJ_NAME} PRIVATE TBB::tbb)