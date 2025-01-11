cmake_minimum_required(VERSION 3.20)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

string(APPEND CMAKE_CXX_FLAGS "-std=c++20 -Wall -Wextra -Werror -pedantic -Wpointer-arith -Wchar-subscripts -Wcomment -Wformat -Wwrite-strings -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function")

project(SinfourmisRenderer)

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

add_executable(
	SinfourmisRenderer
	src/main.cpp
	src/node.cpp
	src/edge.cpp
)

target_link_libraries(SinfourmisRenderer PUBLIC m sfml-graphics sfml-window sfml-system sfml-audio)


include_directories(src)

# json library
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)



# argparser library
FetchContent_Declare(
	argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
	)
FetchContent_MakeAvailable(argparse)

target_link_libraries(SinfourmisRenderer PRIVATE nlohmann_json::nlohmann_json argparse)


