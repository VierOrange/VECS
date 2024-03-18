add_library(example example.cpp )

#target_include_directories(example PUBLIC ${CMAKE_SOURCE_DIR}/include)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_compile_features(example PUBLIC cxx_std_20)
set_target_properties(example PROPERTIES LINKER_LANGUAGE CXX) 
target_link_libraries(example ${SDL2_LIBRARIES})
