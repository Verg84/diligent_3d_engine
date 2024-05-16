cmake_minimum_required (VERSION 3.6)

project(GEOMETRY_SHADER CXX)

set(SOURCE
    src/geometry_shader.cpp
    
)

set(INCLUDE
    src/geometry_shader.hpp
    
)

set(SHADERS
    assets/cube.vsh
    assets/cube.psh
    assets/cube.gsh
    assets/structures.fxh
)



add_sample_app("GEOMETRY_SHADER" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")

