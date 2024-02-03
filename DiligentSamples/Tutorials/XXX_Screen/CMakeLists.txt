cmake_minimum_required (VERSION 3.6)

project(XXX_Screen CXX)

set(SOURCE
    src/XXX_Screen.cpp
)

set(INCLUDE
    src/XXX_Screen.hpp
)

set(SHADERS)
set(ASSETS)

add_sample_app("XXX_Screen" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
