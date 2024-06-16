cmake_minimum_required (VERSION 3.6)

project(MeshShaders_Constants CXX)

set(SOURCE
    src/MeshShaderConstants.cpp
    
)

set(INCLUDE
    src/MeshShaderConstants.hpp
    
)

set(SHADERS
    	assets/MS.msh	
    	assets/PS.psh
	assets/structures.fxh)

set(ASSETS
    
)

add_sample_app("MeshShader_Constants" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
