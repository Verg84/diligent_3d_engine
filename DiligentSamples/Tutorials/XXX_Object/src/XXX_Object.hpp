#pragma once

#include <array>

#include"SampleBase.hpp"
#include "RenderDevice.h"
#include "Buffer.h"
#include "RefCntAutoPtr.hpp"
#include "BasicMath.hpp"

namespace Diligent{
	enum VERTEX_FLAGS :Uint32
	{
		NONE_FLAG=0x00,
		POS_FLAG=0x01,
		NORMAL_FLAG=0x02,
		TEXCOORD_FLAG=0x04,
		POS_UV_FLAG=POS_FLAG|TEXCOORD_FLAG,
		POS_UV_TEX_FLAG=POS_FLAG|NORMAL_FLAG|TEXCOORD_FLAG
	};
	DEFINE_FLAG_ENUM_OPERATORS(VERTEX_FLAGS);

	class Object
	{
		public:
			static constexpr Uint32 NV = 4 * 6;			// cube's vertices
            static constexpr Uint32 NI = 3 * 2 * 6;		// cube's indices
            const std::array<float3, NV> Positions;
            const std::array<float2, NV> TexCoords;
            const std::array<float3, NV> Normals;
            const std::array<Uint32, NI> Indices;

			RefCntAutoPtr<IBuffer> CreateVertexBuffer(IRenderDevice* pDevice, VERTEX_FLAGS VertexFlags, BIND_FLAGS BindFlags = BIND_VERTEX_BUFFER, BUFFER_MODE BufferMode = BUFFER_MODE_UNDEFINED);
            RefCntAutoPtr<IBuffer> CreateIndexBuffer(IRenderDevice* pDevice, BIND_FLAGS BindFlags = BIND_INDEX_BUFFER, BUFFER_MODE BuferMode = BUFFER_MODE_UNDEFINED);
            RefCntAutoPtr<ITexture> LoadTexture(IRenderDevice* pDevice, const char* Path);

			// create pipeline state parameters
			struct CreatePSOInfo
			{
                IRenderDevice* pDevice = nullptr;
                
				TEXTURE_FORMAT pRTV    = TEX_FORMAT_UNKNOWN;
                TEXTURE_FORMAT pDSV    = TEX_FORMAT_UNKNOWN;
                
				IShaderSourceInputStreamFactory *pShaderSourceFactory = nullptr;
                const char*  VSFilePath = nullptr;
				const char* PSFilePath =nullptr;
				
				VERTEX_FLAGS Vertex_Flags=NONE_FLAG;
				LayoutElement* ExtraLayoutElements=nullptr;
                Uint32 NumExtraLayoutElems  = 0;
                Uint8  SampleCount          = 1;
                
			};
            RefCntAutoPtr<IPipelineState> CreatePipelineState(const CreatePSOInfo& pipeline_params);

			
	};
	
}