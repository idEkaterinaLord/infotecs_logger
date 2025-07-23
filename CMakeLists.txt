cmake_minimum_required(VERSION 3.10)
project(infotecs_logger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries (.so)" ON)

add_subdirectory(logger)
add_subdirectory(app)