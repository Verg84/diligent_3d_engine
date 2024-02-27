cmake_minimum_required (VERSION 3.6)

project(XXX_TRIANGLE CXX)

set(SOURCE
    src/triangle.cpp
)

set(INCLUDE
    src/triangle.hpp
)

set(SHADERS
    assets/simple_vertex_shader.vsh
    assets/simple_pixel_shader.psh
)

set(ASSETS)

add_sample_app("XXX_TRIANGLE" "DiligentSamples/Tutorials/XXX_PROJECTS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
