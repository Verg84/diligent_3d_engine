cmake_minimum_required (VERSION 3.6)

project(XXX_Textures CXX)

set(SOURCE
    src/XXX_Textures.cpp
)

set(INCLUDE
    src/XXX_Textures.hpp
)

set(SHADERS
    assets/textures_vertex_shader_source.vsh
    assets/textures_pixel_shader_source.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("XXX_Textures" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(XXX_Textures PRIVATE Diligent-TextureLoader)