cmake_minimum_required (VERSION 3.6)

project(Instancing CXX)

set(SOURCE
    src/Instancing.cpp
    
)

set(INCLUDE
    src/Instancing.hpp
    
)

set(SHADERS
    assets/cube_inst.vsh
    assets/cube_inst.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("Instancing" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(Instancing PRIVATE Diligent-TextureLoader)