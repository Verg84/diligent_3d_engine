cmake_minimum_required (VERSION 3.6)

project(VERTEX_BUFFERS CXX)

set(SOURCE
    src/vertex_buffers.cpp
)

set(INCLUDE
    src/vertex_buffers.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS)

add_sample_app("VERTEX_BUFFERS" "DiligentSamples/Tutorials/XXX_GLSL/" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
