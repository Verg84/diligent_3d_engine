cmake_minimum_required (VERSION 3.6)

project(GLSLLoading CXX)

set(SOURCE
    src/glsl_loading.cpp
)

set(INCLUDE
    src/glsl_loading.hpp
)

set(SHADERS
    assets/VS.glsl
    assets/PS.glsl
)

set(ASSETS)

add_sample_app("GLSLLoading" "DiligentSamples/Tutorials/XXX_GLSL/" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
