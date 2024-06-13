cmake_minimum_required (VERSION 3.6)

project(MeshShaderIntro CXX)

set(SOURCE
    src/MeshShader.cpp
    
)

set(INCLUDE
    src/MeshShader.hpp
    
)

set(SHADERS
    	assets/MS.msh
    	assets/PS.psh
	assets/MS001.msh
	assets/PS001.psh
	assets/buildMS.msh
	assets/buildPS.psh
    
)

set(ASSETS
   
)

add_sample_app("MeshShaderIntro" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
