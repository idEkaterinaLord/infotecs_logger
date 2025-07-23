add_executable(logger_app main.cpp)

target_link_libraries(logger_app logger)

target_include_directories(logger_app PRIVATE
    ${PROJECT_SOURCE_DIR}/logger/include
    ${CMAKE_CURRENT_SOURCE_DIR}  # для SafeQueue.hpp
)
