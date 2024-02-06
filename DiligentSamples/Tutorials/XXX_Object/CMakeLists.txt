cmake_minimum_required (VERSION 3.6)

project(XXX_Object CXX)

set(SOURCE
    src/XXX_Object.cpp
)

set(INCLUDE
    src/XXX_Object.hpp
)

set(SHADERS
    assets/vertex_shader_source.vsh
    assets/fragment_shader_source.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("XXX_Object" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(XXX_Object PRIVATE Diligent-TextureLoader)