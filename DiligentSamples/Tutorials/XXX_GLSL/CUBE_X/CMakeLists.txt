cmake_minimum_required (VERSION 3.6)

project(CUBE_X CXX)

set(SOURCE
    src/cube.cpp
)

set(INCLUDE
    src/cube.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS)

add_sample_app("CUBE_X" "DiligentSamples/Tutorials/XXX_GLSL/" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
