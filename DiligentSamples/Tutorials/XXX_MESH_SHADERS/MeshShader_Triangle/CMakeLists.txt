cmake_minimum_required (VERSION 3.6)

project(MeshShader_Triangle CXX)

set(SOURCE
    src/MeshShaderTriangle.cpp
    
)

set(INCLUDE
    src/MeshShaderTriangle.hpp
    
)

set(SHADERS
    	assets/MS.msh	
    	assets/PS.psh
	assets/structures.fxh)

set(ASSETS
    
)

add_sample_app("MeshShader_Triangle" "DiligentSamples/Tutorials/XXX_MESH_SHADERS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
