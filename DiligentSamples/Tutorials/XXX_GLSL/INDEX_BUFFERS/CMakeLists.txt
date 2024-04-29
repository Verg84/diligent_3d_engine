cmake_minimum_required (VERSION 3.6)

project(INDEX_BUFFERS CXX)

set(SOURCE
    src/index_buffers.cpp
)

set(INCLUDE
    src/index_buffers.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS)

add_sample_app("INDEX_BUFFERS" "DiligentSamples/Tutorials/XXX_GLSL/" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
