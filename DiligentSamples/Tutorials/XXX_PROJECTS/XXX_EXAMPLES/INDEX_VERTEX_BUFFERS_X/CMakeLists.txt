cmake_minimum_required (VERSION 3.6)

project(INDEX_VERTEX_BUFFERS_X CXX)

set(SOURCE
    src/indexing.cpp
)

set(INCLUDE
    src/indexing.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS)

add_sample_app("INDEX_VERTEX_BUFFERS_X" "DiligentSamples/Tutorials/XXX_PROJECTS/XXX_EXAMPLES" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
