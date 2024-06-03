cmake_minimum_required (VERSION 3.6)

project(GeometryShader001 CXX)

set(SOURCE
    src/GeometryShader001.cpp
    
)

set(INCLUDE
    src/GeometryShader001.hpp
   
)

set(SHADERS
    assets/cube.vsh
    assets/cube.psh
    assets/cube.gsh
    assets/structures.fxh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("GeometryShader001" "DiligentSamples/Tutorials/XXX_ADV_SHADERS/HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")

