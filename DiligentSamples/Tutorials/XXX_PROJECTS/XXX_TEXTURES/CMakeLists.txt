cmake_minimum_required (VERSION 3.6)

project(TEXTURES CXX)

set(SOURCE
    src/Textures.cpp
)

set(INCLUDE
    src/Textures.hpp
)

set(SHADERS
    assets/texture_vertex_shader_source.vsh
    assets/texture_pixel_shader_source.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("TEXTURES" "DiligentSamples/Tutorials/XXX_PROJECTS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(TEXTURES PRIVATE Diligent-TextureLoader)