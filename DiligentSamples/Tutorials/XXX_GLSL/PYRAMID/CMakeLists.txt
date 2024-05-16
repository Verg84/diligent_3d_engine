cmake_minimum_required (VERSION 3.6)

project(PYRAMID CXX)

set(SOURCE
    src/pyramid.cpp
)

set(INCLUDE
    src/pyramid.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS
    assets/pyramid_texture.png
)

add_sample_app("PYRAMID" "DiligentSamples/Tutorials/XXX_GLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(PYRAMID PRIVATE Diligent-TextureLoader)