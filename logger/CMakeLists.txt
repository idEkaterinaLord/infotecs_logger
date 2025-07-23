add_library(logger
    src/FileLogger.cpp
    src/SocketLogger.cpp
)

target_include_directories(logger
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)