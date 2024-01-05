file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/ECS/*.h")

add_library(ecs ecs.cpp entity.cpp component.cpp system.cpp ${HEADER_LIST})

target_include_directories(ecs PUBLIC ../include)
target_compile_features(ecs PUBLIC cxx_std_20)
