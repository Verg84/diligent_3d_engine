cmake_minimum_required (VERSION 3.6)

project(MeshShaders_DataLoad CXX)

set(SOURCE
    src/MeshShaderDataLoad.cpp
    
)

set(INCLUDE
    src/MeshShaderDataLoad.hpp
    
)

set(SHADERS
    	assets/MS.msh	
    	assets/PS.psh
	assets/structures.fxh)

set(ASSETS
    
)

add_sample_app("MeshShader_DataLoad" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
