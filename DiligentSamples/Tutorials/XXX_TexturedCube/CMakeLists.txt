cmake_minimum_required (VERSION 3.6)

project(XXX_TexturedCube CXX)

set(SOURCE
    src/XXX_TexturedCube.cpp
)

set(INCLUDE
    src/XXX_TexturedCube.hpp
)

set(SHADERS
    assets/cube.vsh
    assets/cube.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("XXX_TexturedCube" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(XXX_TexturedCube PRIVATE Diligent-TextureLoader)