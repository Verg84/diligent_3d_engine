cmake_minimum_required (VERSION 3.6)

project(User_HelloTriangle CXX)

set(SOURCE
    src/Tutorial01_HelloTriangle.cpp
)

set(INCLUDE
    src/Tutorial01_HelloTriangle.hpp
)

set(SHADERS)
set(ASSETS)

add_sample_app("User_HelloTriangle" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
