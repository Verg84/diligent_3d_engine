cmake_minimum_required (VERSION 3.6)

project(LAYOUT_ELEMENTS CXX)

set(SOURCE
    src/Layout_Elements.cpp
)

set(INCLUDE
    src/Layout_Elements.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS)

add_sample_app("LAYOUT_ELEMENTS" "DiligentSamples/Tutorials/XXX_PROJECTS/XXX_EXAMPLES" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
