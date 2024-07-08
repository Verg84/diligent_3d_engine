cmake_minimum_required (VERSION 3.6)

project(MeshShader_Cube3d CXX)

set(SOURCE
    src/MeshShaderCube3d.cpp
    
)

set(INCLUDE
    src/MeshShaderCube3d.hpp
    
)

set(SHADERS
    	assets/MS.msh	
    	assets/PS.psh
	assets/structures.fxh)

set(ASSETS
    
)

add_sample_app("MeshShader_Cube3d" "DiligentSamples/Tutorials/XXX_MESH_SHADERS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
