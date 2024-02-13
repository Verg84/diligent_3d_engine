cmake_minimum_required (VERSION 3.6)

project(Tutorial07_GeometryShader CXX)

set(SOURCE
    src/XXX_GeometryShader.cpp
    ../Common/src/TexturedCube.cpp
)

set(INCLUDE
    src/XXX_GeometryShader.hpp
    ../Common/src/TexturedCube.hpp
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

add_sample_app("XXX_GeometryShader" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(XXX_GeometryShader PRIVATE Diligent-TextureLoader)
