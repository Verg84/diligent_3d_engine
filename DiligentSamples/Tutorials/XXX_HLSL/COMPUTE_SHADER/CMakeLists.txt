cmake_minimum_required (VERSION 3.6)

project(Compute_Shader CXX)

set(SOURCE
    src/ComputeShader.cpp
)

set(INCLUDE
    src/ComputeShader.hpp
)

set(SHADERS
    assets/particle.psh
    assets/particle.vsh
    assets/structures.fxh
    assets/reset_particle_lists.csh
    assets/collide_particles.csh
    assets/move_particles.csh
    assets/particles.fxh
)

set(ASSETS)

add_sample_app("Compute_Shader" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
