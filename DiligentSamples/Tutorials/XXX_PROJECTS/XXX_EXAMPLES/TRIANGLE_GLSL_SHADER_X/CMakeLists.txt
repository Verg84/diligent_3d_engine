cmake_minimum_required (VERSION 3.6)

project(TRIANGLE_GLSL CXX)

set(SOURCE
    src/triangle_glsl.cpp
)

set(INCLUDE
    src/triangle_glsl.hpp
)

set(SHADERS
    assets/VS.vert
    assets/PS.frag
)

set(ASSETS)

add_sample_app("TRIANGLE_GLSL" "DiligentSamples/Tutorials/XXX_PROJECTS/XXX_EXAMPLES" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
