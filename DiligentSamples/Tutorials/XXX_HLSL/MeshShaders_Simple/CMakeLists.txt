cmake_minimum_required (VERSION 3.6)

project(MeshShaders_Simple CXX)

set(SOURCE
    src/MeshShaderSimple.cpp
    
)

set(INCLUDE
    src/MeshShaderSimple.hpp
    
)

set(SHADERS
    	assets/simpleMS001.msh
    	assets/simpleMS002.msh
	assets/simpleMS003.msh	
    	assets/simplePS.psh)

set(ASSETS
    
)

add_sample_app("MeshShader_Sample" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
