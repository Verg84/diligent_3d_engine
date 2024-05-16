cmake_minimum_required (VERSION 3.6)

project(TEXTURING_INST CXX)

set(SOURCE
    src/texturingInst.cpp
)

set(INCLUDE
    src/texturingInst.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS
    assets/NoExit.jpg
)

add_sample_app("TEXTURING_INST" "DiligentSamples/Tutorials/XXX_GLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(TEXTURING_INST PRIVATE Diligent-TextureLoader)