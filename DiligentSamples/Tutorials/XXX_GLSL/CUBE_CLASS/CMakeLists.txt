cmake_minimum_required (VERSION 3.6)

project(CUBE_CLASS CXX)

set(SOURCE
    src/cube_class.cpp
)

set(INCLUDE
    src/cube_class.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS)

add_sample_app("CUBE_CLASS" "DiligentSamples/Tutorials/XXX_GLSL/" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
