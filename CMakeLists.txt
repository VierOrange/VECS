cmake_minimum_required(VERSION 3.14)
project(
		VECS
		VERSION 0.1
		DESCRIPTION "An ECS from VierOrange"
		LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

include_directories(include)

add_subdirectory(examples)
add_subdirectory(src)
add_subdirectory(tests EXCLUDE_FROM_ALL)
