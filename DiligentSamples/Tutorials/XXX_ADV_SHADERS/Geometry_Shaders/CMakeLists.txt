cmake_minimum_required (VERSION 3.6)

project(Geometry_Shaders CXX)

set(SOURCE
    src/GeometryShader.cpp
)

set(INCLUDE
    src/GeometryShader.hpp
    
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

add_sample_app("Geometry_Shaders" "DiligentSamples/Tutorials/XXX_ADV_SHADERS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")

