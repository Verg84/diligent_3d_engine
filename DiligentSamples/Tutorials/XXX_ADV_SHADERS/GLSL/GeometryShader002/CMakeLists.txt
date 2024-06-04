cmake_minimum_required (VERSION 3.6)

project(GeomShader002 CXX)

set(SOURCE
    src/GeomShader002.cpp
)

set(INCLUDE
    src/GeomShader002.hpp
    
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
    assets/GS.glsl
    
)

set(ASSETS
    
)

add_sample_app("GeomShader002" "DiligentSamples/Tutorials/XXX_ADV_SHADERS/GLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")

