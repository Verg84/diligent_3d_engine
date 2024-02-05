cmake_minimum_required (VERSION 3.6)

project(XXX_Various CXX)

set(SOURCE
    src/XXX_Various.cpp
)

set(INCLUDE
    src/XXX_Various.hpp
)

set(SHADERS
    assets/vertex_shader_source.vsh
    assets/fragment_shader_source.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("XXX_Various" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(XXX_Various PRIVATE Diligent-TextureLoader)