cmake_minimum_required (VERSION 3.6)

project(XXX_Mesh CXX)

set(SOURCE
    src/Mesh.cpp
)

set(INCLUDE
    src/Mesh.hpp
)

set(SHADERS
    assets/vertex_shader_source.vsh
    assets/pixel_shader_source.psh
)

set(ASSETS
    assets/DGLogo.png
	assets/spider.off
)

add_sample_app("XXX_Mesh" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(XXX_Mesh PRIVATE Diligent-TextureLoader)