cmake_minimum_required (VERSION 3.6)

project(UNIFORM_BUFFERS CXX)

set(SOURCE
    src/uniform_buffers.cpp
)

set(INCLUDE
    src/uniform_buffers.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS)

add_sample_app("UNIFORM_BUFFERS" "DiligentSamples/Tutorials/XXX_PROJECTS/XXX_EXAMPLES" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
