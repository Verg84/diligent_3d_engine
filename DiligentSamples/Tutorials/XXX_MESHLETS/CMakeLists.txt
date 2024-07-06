cmake_minimum_required (VERSION 3.6)

project(Meshlets CXX)

set(SOURCE
    src/BaseRenderer.cpp
    
)

set(INCLUDE
    src/BaseRenderer.hpp
    
)

set(SHADERS
    	
		assets/MS.msh
    	assets/PS.psh
    	)

set(ASSETS
    
)

add_sample_app("Meshlets" "DiligentSamples/Tutorials/XXX_MESHLETS" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
