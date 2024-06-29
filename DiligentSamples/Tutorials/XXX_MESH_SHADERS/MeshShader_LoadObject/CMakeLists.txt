cmake_minimum_required (VERSION 3.6)

project(MeshShader_LoadObject CXX)

set(SOURCE
    src/MeshShader.cpp
    
)

set(INCLUDE
    src/MeshShader.hpp
    
)

set(SHADERS
    assets/VS.vsh
    assets/PS.psh
	assets/cube.off
		
    	)

set(ASSETS
    
)

add_sample_app("MeshShader_LoadObject" "DiligentSamples/Tutorials/XXX_MESH_SHADERS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
