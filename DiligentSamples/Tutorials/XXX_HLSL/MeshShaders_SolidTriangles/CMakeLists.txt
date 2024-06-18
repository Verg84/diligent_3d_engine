cmake_minimum_required (VERSION 3.6)

project(MeshShaders_SolidTriangles CXX)

set(SOURCE
    src/MeshShaderSolids.cpp
    
)

set(INCLUDE
    src/MeshShaderSolids.hpp
    
)

set(SHADERS
    	assets/MS.msh	
    	assets/PS.psh
	assets/uvMS.msh
	assets/structures.fxh)

set(ASSETS
    
)

add_sample_app("MeshShader_SolidTriangles" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
