file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/VECS/core/*.hpp")

add_library(ecs ${HEADER_LIST})

target_include_directories(ecs PUBLIC ../include/)
target_compile_features(ecs PUBLIC cxx_std_20)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE CXX) 
