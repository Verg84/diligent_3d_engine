cmake_minimum_required (VERSION 3.6)

project(MeshShader_PassThrough CXX)

set(SOURCE
    src/MeshShader.cpp
    
)

set(INCLUDE
    src/MeshShader.hpp
    
)

set(SHADERS
    	assets/pass_through001_MS.msh
	assets/pass_through002_MS.msh
	assets/pass_through003_MS.msh
    	assets/PS.psh
		
    	)

set(ASSETS
    
)

add_sample_app("MeshShader_PassThrough" "DiligentSamples/Tutorials/XXX_MESH_SHADERS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
