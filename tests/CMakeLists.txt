add_executable(testecs testECS.cpp)
target_link_libraries(testecs PRIVATE vecs)
target_compile_features(testecs PRIVATE cxx_std_20)
