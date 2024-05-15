cmake_minimum_required (VERSION 3.6)

project(Instancing_hlsl CXX)

set(SOURCE
    src/Instancing_hlsl.cpp
    
)

set(INCLUDE
    src/Instancing_hlsl.hpp
    
)

set(SHADERS
    assets/cube_inst.vsh
    assets/cube_inst.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("Instancing_hlsl" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(Instancing_hlsl PRIVATE Diligent-TextureLoader)