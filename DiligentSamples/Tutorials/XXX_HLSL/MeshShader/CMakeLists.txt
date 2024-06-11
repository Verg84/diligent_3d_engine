cmake_minimum_required (VERSION 3.6)

project(MESH_SHADER CXX)

set(SOURCE
    src/meshObject.cpp
    
)

set(INCLUDE
    src/meshObject.hpp
	src/Object.hpp
    
)

set(SHADERS
	assets/lettersPS.psh
	assets/lettersMS.msh
	assets/simpleMS.msh
	assets/simplePS.psh
	assets/pass_throughMS.msh
	assets/pass_throughPS.psh
    
)



add_sample_app("MESH_SHADER" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")

