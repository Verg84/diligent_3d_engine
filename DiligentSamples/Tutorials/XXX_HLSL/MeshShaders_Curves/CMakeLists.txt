cmake_minimum_required (VERSION 3.6)

project(MeshShaders_Curves CXX)

set(SOURCE
    src/MeshShaderCurves.cpp
    
)

set(INCLUDE
    src/MeshShaderCurves.hpp
    
)

set(SHADERS
    	assets/MS.msh	
    	assets/PS.psh
)	

set(ASSETS
    
)

add_sample_app("MeshShaders_Curves" "DiligentSamples/Tutorials/XXX_HLSL" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
