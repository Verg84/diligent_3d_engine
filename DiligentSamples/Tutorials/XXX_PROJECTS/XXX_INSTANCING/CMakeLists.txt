cmake_minimum_required (VERSION 3.6)

project(INSTANCING CXX)

set(SOURCE
    src/Instancing.cpp
    
)

set(INCLUDE
    src/Instancing.hpp
    
)

set(SHADERS
    assets/texture_vertex_shader_source.vsh
    assets/texture_pixel_shader_source.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("INSTANCING" "DiligentSamples/Tutorials/XXX_PROJECTS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(INSTANCING PRIVATE Diligent-TextureLoader)