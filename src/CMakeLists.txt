file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/VECS/core/*.hpp")

add_library(vecs ${HEADER_LIST})

target_include_directories(vecs PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(vecs PUBLIC cxx_std_20)
set_target_properties(vecs PROPERTIES LINKER_LANGUAGE CXX) 
