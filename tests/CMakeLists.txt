add_executable(testvecs TestVECS.cpp)
target_include_directories(testvecs PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(testvecs PRIVATE cxx_std_20)
