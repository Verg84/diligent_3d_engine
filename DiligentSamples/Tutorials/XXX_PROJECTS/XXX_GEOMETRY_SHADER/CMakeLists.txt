cmake_minimum_required (VERSION 3.6)

project(GeometryShader CXX)

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

add_sample_app("GeometryShader" "DiligentSamples/Tutorials/XXX_PROJECTS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(GeometryShader PRIVATE Diligent-TextureLoader)
