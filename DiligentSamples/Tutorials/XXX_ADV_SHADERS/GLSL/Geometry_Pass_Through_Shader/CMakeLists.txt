cmake_minimum_required (VERSION 3.6)

project(GeometryShader_PassThrought CXX)

set(SOURCE
    src/GeometryShader_PassThrought.cpp
)

set(INCLUDE
    src/GeometryShader_PassThrought.hpp
    
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
    assets/GS.glsl
    
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("GeometryShader_PassThrought" "DiligentSamples/Tutorials/XXX_ADV_SHADERS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")

