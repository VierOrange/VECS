cmake_minimum_required(VERSION 3.2)
project(ESC)

set(SOURCES entity.cpp component.cpp system.cpp)

set(LIBRARY_NAME ecs)
set(EXECUTABLE_NAME play)


add_library(${LIBRARY_NAME} ${SOURCES})
add_executable(${EXECUTABLE_NAME} play.cpp)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${LIBRARY_NAME})
target_compile_options(play PUBLIC -std=c++20 -Wall -Wfloat-conversion)
