cmake_minimum_required (VERSION 3.6)

project(XXX_TextureArrays CXX)

set(SOURCE
    src/XXX_TextureArrays.cpp
    ../Common/src/TexturedCube.cpp
)

set(INCLUDE
    src/XXX_TextureArrays.hpp
    ../Common/src/TexturedCube.hpp
)

set(SHADERS
    assets/inst_texarray_vertex_shaderSrc.vsh
    assets/inst_texarray_pixel_shaderSrc.psh
)

set(ASSETS
    assets/DGLogo0.png
    assets/DGLogo1.png
    assets/DGLogo2.png
    assets/DGLogo3.png
)

add_sample_app("XXX_TextureArrays" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(XXX_TextureArrays PRIVATE Diligent-TextureLoader)