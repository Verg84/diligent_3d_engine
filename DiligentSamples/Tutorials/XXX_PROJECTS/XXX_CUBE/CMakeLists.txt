cmake_minimum_required (VERSION 3.6)

project(Cube CXX)

set(SOURCE
    src/Cube.cpp
)

set(INCLUDE
    src/Cube.hpp
)

set(SHADERS
    assets/vertex_shader_source.vsh
    assets/pixel_shader_source.psh
)

set(ASSETS)

add_sample_app("Cube" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
