cmake_minimum_required (VERSION 3.6)

project(MESH_SHADER CXX)

set(SOURCE
    src/MeshShader.cpp
    
)

set(INCLUDE
    src/MeshShader.hpp
  
)

set(SHADERS
    assets/cube.ash
    assets/cube.msh
    assets/cube.psh
    assets/structures.fxh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("MeshShader" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
