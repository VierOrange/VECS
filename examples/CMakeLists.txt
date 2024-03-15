add_library(example example.cpp)

#target_include_directories(example PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(example PUBLIC cxx_std_20)
set_target_properties(example PROPERTIES LINKER_LANGUAGE CXX) 
