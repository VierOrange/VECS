cmake_minimum_required(VERSION 3.14)
project(
		ECS
		VERSION 0.1
		DESCRIPTION "An ECS using SIMD"
		LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

add_subdirectory(tests EXCLUDE_FROM_ALL)
add_subdirectory(src)
