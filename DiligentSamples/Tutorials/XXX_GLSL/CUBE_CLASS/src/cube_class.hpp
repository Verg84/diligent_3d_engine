#pragma once

#include<array>

#include"RenderDevice.h"
#include"Buffer.h"
#include"RefCntAutoPtr.hpp"
#include"BasicMath.hpp"

namespace Diligent
{
	namespace CubeClass
	{
		enum VERTEX_COMPONENTS_FLAGS :Uint32
		{
            VERTEX_COMPONENT_FLAG_NONE     = 0x00,
			VERTEX_COMPONENT_FLAG_POSITION=0x01,
			VERTEX_COMPONENT_FLAG_NORMAL=0x02,
			VERTEX_COMPONENT_FLAG_TEXCOORD=0x04,

			VERTEX_COMPONENT_FLAG_POS_UV=VERTEX_COMPONENT_FLAG_POSITION | VERTEX_COMPONENT_FLAG_TEXCOORD,
			VERTEX_COMPONENT_FLAG_POS_NORM_UV=VERTEX_COMPONENT_FLAG_POSITION|VERTEX_COMPONENT_FLAG_TEXCOORD|VERTEX_COMPONENT_FLAG_NORMAL
    };
    DEFINE_FLAG_ENUM_OPERATORS(VERTEX_COMPONENTS_FLAGS);

	RefCntAutoPtr<IBuffer> CreateVertexBuffer(
        IRenderDevice*          pDevice,
        VERTEX_COMPONENTS_FLAGS Components,
        BIND_FLAGS              BindFlags = BIND_VERTEX_BUFFER,
        BUFFER_MODE             Mode      = BUFFER_MODE_UNDEFINED);

    RefCntAutoPtr<IBuffer> CreateIndexBuffer(
        IRenderDevice* pDevice,
        BIND_FLAGS     BindFlags = BIND_INDEX_BUFFER,
        BUFFER_MODE    Mode      = BUFFER_MODE_UNDEFINED);

	RefCntAutoPtr<ITexture> LoadTexture(IRenderDevice* pDevice, const char* Path);

	struct PSOCreateInfo
	{
        IRenderDevice* pDevice = nullptr;
        
        TEXTURE_FORMAT RTVFormat = TEX_FORMAT_UNKNOWN;
        TEXTURE_FORMAT DSVFormat = TEX_FORMAT_UNKNOWN;
        
        IShaderSourceInputStreamFactory* pShaderSourceFactory = nullptr;
        const char*                      VSFilePath           = nullptr;
        const char*                      PSFilePath           = nullptr;
        
        VERTEX_COMPONENTS_FLAGS          Components           = VERTEX_COMPONENT_FLAG_NONE;
        LayoutElement* ExtraLayoutElements=nullptr;
        Uint32                           NumExtraLayoutElements = 0;
        Uint8                            SampleCount            = 1;
	};
    RefCntAutoPtr<IPipelineState> CreatePipelineState(const PSOCreateInfo& CreateInfo);

    static constexpr Uint32 NumVertices=4*6;
    static constexpr Uint32 NumIndices=3*2*6;

    extern const std::array<float3,NumVertices> Positions;
    extern const std::array<float2, NumVertices> TexCoords;
    extern const std::array<float3, NumVertices> Normals;

    extern const std::array<Uint32,NumIndices> Indices;

	}// namespace CubeClass
}// namespace Diligent