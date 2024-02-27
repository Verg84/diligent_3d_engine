cmake_minimum_required (VERSION 3.6)

project(TEXARRAY CXX)

set(SOURCE
    src/TexArray.cpp
    
)

set(INCLUDE
    src/TexArray.hpp
    
)

set(SHADERS
    assets/texarray_inst.vsh
    assets/texarray_inst.psh
)

set(ASSETS
    assets/DGLogo0.png
    assets/DGLogo1.png
    assets/DGLogo2.png
    assets/DGLogo3.png
)

add_sample_app("TEXARRAY" "DiligentSamples/Tutorials/XXX_PROJECTS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(TEXARRAY PRIVATE Diligent-TextureLoader)