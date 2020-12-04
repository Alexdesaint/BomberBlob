add_library(Core STATIC
        StaticCube.cpp
        DynamicCube.cpp
        ContactListener.cpp PerlinNoise.cpp StaticCollider.cpp FunctionPlane.cpp DynamicCollider.cpp)

target_compile_definitions(Core PUBLIC B2_USER_SETTINGS)

target_link_libraries(Core Blob box2d)

