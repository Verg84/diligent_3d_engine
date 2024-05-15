cmake_minimum_required (VERSION 3.6)

project(CUBE_ARRAYS CXX)

set(SOURCE
    src/cube_arrays.cpp
)

set(INCLUDE
    src/cube_arrays.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS)

add_sample_app("CUBE_ARRAYS" "DiligentSamples/Tutorials/XXX_GLSL/" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
