cmake_minimum_required (VERSION 3.6)

project(XXX_Instancing CXX)

set(SOURCE
    src/XXX_Instancing.cpp
    ../Common/src/TexturedCube.cpp
)

set(INCLUDE
    src/XXX_Instancing.hpp
    ../Common/src/TexturedCube.hpp
)

set(SHADERS
    assets/cube_inst.vsh
    assets/cube_inst.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("XXX_Instancing" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(XXX_Instancing PRIVATE Diligent-TextureLoader)