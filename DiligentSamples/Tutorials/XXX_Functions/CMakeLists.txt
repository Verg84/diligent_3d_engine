cmake_minimum_required (VERSION 3.6)

project(XXX_Functions CXX)

set(SOURCE
    src/XXX_Functions.cpp
)

set(INCLUDE
    src/XXX_Functions.hpp
)

set(SHADERS
    assets/vertex_shader_source.vsh
    assets/pixel_shader_source.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("XXX_Functions" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(XXX_Functions PRIVATE Diligent-TextureLoader)