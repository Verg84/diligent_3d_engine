cmake_minimum_required (VERSION 3.6)

project(XXX_TRIANGLE CXX)

set(SOURCE
    src/triangle.cpp
)

set(INCLUDE
    src/triangle.hpp
)

set(SHADERS
    assets/VS.vsh
    assets/PS.psh
)

set(ASSETS)

add_sample_app("TRIANGLE_X" "DiligentSamples/Tutorials/XXX_PROJECTS/XXX_EXAMPLES" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
