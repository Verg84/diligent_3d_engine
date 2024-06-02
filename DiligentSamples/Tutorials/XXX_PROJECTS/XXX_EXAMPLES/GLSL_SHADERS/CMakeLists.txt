cmake_minimum_required (VERSION 3.6)

project(LOADING_GLSL CXX)

set(SOURCE
    src/loading_glsl.cpp
)

set(INCLUDE
    src/loading_glsl.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS)

add_sample_app("LOADING_GLSL" "DiligentSamples/Tutorials/XXX_PROJECTS/XXX_EXAMPLES" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
