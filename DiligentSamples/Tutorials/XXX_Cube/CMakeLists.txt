cmake_minimum_required (VERSION 3.6)

project(XXX_Cube CXX)

set(SOURCE
    src/XXX_Cube.cpp
)

set(INCLUDE
    src/XXX_Cube.hpp
)

set(SHADERS
    assets/cube.vsh
    assets/cube.psh
)

set(ASSETS)

add_sample_app("XXX_Cube" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
