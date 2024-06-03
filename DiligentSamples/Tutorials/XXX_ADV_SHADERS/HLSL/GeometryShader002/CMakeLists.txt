cmake_minimum_required (VERSION 3.6)

project(GeometryShader002 CXX)

set(SOURCE
    src/GeometryShader002.cpp
    
)

set(INCLUDE
    src/GeometryShader002.hpp
   
)

set(SHADERS
    assets/cube.vsh
    assets/cube.psh
    assets/cube.gsh
    
)

set(ASSETS
    
)

add_sample_app("GeometryShader002" "DiligentSamples/Tutorials/XXX_ADV_SHADERS/HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")

