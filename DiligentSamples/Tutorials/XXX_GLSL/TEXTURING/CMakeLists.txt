cmake_minimum_required (VERSION 3.6)

project(TEXTURING CXX)

set(SOURCE
    src/texturing.cpp
)

set(INCLUDE
    src/texturing.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("TEXTURING" "DiligentSamples/Tutorials/XXX_GLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(TEXTURING PRIVATE Diligent-TextureLoader)