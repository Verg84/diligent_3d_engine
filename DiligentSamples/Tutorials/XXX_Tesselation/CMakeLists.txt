cmake_minimum_required (VERSION 3.6)

project(XXX_Tessellation CXX)

set(SOURCE
    src/Tessellation.cpp
)

set(INCLUDE
    src/Tessellation.hpp
)

set(SHADERS
    assets/terrain.vsh
    assets/terrain.psh
    assets/terrain_wire.psh
    assets/terrain.gsh
    assets/terrain.hsh
    assets/terrain.dsh
    assets/structures.fxh
)

set(ASSETS
    assets/ps_height_1k.png
    assets/ps_texture_2k.png
)

add_sample_app("XXX_Tesselation" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
target_link_libraries(XXX_Tesselation PRIVATE Diligent-TextureLoader)
