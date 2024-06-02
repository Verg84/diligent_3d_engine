cmake_minimum_required (VERSION 3.6)

project(TEXTURES_X CXX)

set(SOURCE
    src/textures.cpp
)

set(INCLUDE
    src/textures.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("TEXTURES_X" "DiligentSamples/Tutorials/XXX_GLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(TEXTURES_X PRIVATE Diligent-TextureLoader)