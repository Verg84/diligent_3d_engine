cmake_minimum_required (VERSION 3.6)

project(MeshShaders_Tut CXX)

set(SOURCE
    src/MeshShader.cpp
    
)

set(INCLUDE
    src/MeshShader.hpp
    
)

set(SHADERS
    	assets/MS.msh	
    	assets/PS.psh
	assets/AS.ash
	assets/structures.fxh)

set(ASSETS
    
)

add_sample_app("MeshShaders_Tut" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
