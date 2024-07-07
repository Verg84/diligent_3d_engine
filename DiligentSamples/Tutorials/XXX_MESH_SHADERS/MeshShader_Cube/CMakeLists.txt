cmake_minimum_required (VERSION 3.6)

project(MeshShader_Cube CXX)

set(SOURCE
    src/MeshShaderCube.cpp
    
)

set(INCLUDE
    src/MeshShaderCube.hpp
    
)

set(SHADERS
    	assets/MS.msh	
    	assets/PS.psh
	assets/structures.fxh)

set(ASSETS
    
)

add_sample_app("MeshShader_Cube" "DiligentSamples/Tutorials/XXX_MESH_SHADERS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
