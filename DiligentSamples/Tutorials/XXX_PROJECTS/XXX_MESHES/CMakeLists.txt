cmake_minimum_required (VERSION 3.6)

project(ASS_01 CXX)

set(SOURCE
    src/ass_01.cpp
)

set(INCLUDE
    src/ass_01.hpp
)

set(SHADERS
    assets/simple_vertex_shader_source.vert
    assets/simple_pixel_shader_source.frag
)

set(ASSETS)

add_sample_app("ASS_01" "DiligentSamples/Tutorials/XXX_PROJECTS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
