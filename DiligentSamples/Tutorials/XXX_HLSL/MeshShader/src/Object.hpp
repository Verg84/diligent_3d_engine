#pragma once

#include<array>
#include<vector>

#include"RenderDevice.h"
#include"Buffer.h"
#include"RefCntAutoPtr.hpp"
#include"BasicMath.hpp"
#include "GraphicsTypesX.hpp"

namespace Diligent
{
	namespace Object
	{
        static constexpr Uint32 NumVertices = 3;
        static constexpr Uint32 NumIndices  = 3 * 1;

       
	    const std::array<float3, NumVertices> Positions =
            {
                float3(0.0f, 0.0f, 0.0f), float3(1.0f, 0.0, 0.0), float3(0.0, 1.0, 0.0)};
        const std::array<Uint32, NumIndices> Indices =
            {
                0, 1, 2};

		enum VERTEX_COMPONENT_FLAGS :Uint32
		{
			VERTEX_COMPONENT_FLAG_NONE=0x00,
			VERTEX_COMPONENT_FLAG_POSITION=0x01
		};
        DEFINE_FLAG_ENUM_OPERATORS(VERTEX_COMPONENT_FLAGS);

        RefCntAutoPtr<IBuffer> CreateVertexBuffer(
            IRenderDevice* pDevice,
            VERTEX_COMPONENT_FLAGS Components,
            BIND_FLAGS             BindFlags = BIND_VERTEX_BUFFER,
            BUFFER_MODE            Mode = BUFFER_MODE_UNDEFINED)
        {
            VERIFY_EXPR(Components != VERTEX_COMPONENT_FLAG_NONE);
            const Uint32 TotalVertexComponents = ((Components & VERTEX_COMPONENT_FLAG_POSITION) ? 3 : 0);
            
            std::vector<float> VertexData(size_t{TotalVertexComponents} * NumVertices);
            auto               it = VertexData.begin();
            for (Uint32 v = 0; v < NumVertices; ++v)
            {
                if (Components & VERTEX_COMPONENT_FLAG_POSITION)
                {
                    const auto& Pos{Positions[v]};
                    *(it++) = Pos.x;
                    *(it++) = Pos.y;
                    *(it++) = Pos.z;
                }
            }
            VERIFY_EXPR(it == VertexData.end());

            BufferDesc VertBuffDesc;
            VertBuffDesc.Name      = "Cube vertex buffer";
            VertBuffDesc.Usage     = USAGE_IMMUTABLE;
            VertBuffDesc.BindFlags = BindFlags;
            VertBuffDesc.Size      = static_cast<Uint64>(VertexData.size() * sizeof(VertexData[0]));
            VertBuffDesc.Mode      = Mode;
            if (Mode != BUFFER_MODE_UNDEFINED)
            {
                VertBuffDesc.ElementByteStride = TotalVertexComponents * sizeof(VertexData[0]);
            }

            BufferData VBData;
            VBData.pData    = VertexData.data();
            VBData.DataSize = VertBuffDesc.Size;
            RefCntAutoPtr<IBuffer> pCubeVertexBuffer;

            pDevice->CreateBuffer(VertBuffDesc, &VBData, &pCubeVertexBuffer);

            return pCubeVertexBuffer;
        }

        RefCntAutoPtr<IBuffer> CreateIndexBuffer(
            IRenderDevice* pDevice,
            BIND_FLAGS     BindFlags = BIND_INDEX_BUFFER,
            BUFFER_MODE    Mode = BUFFER_MODE_UNDEFINED)
        {
            BufferDesc IndBuffDesc;
            IndBuffDesc.Name      = "Cube index buffer";
            IndBuffDesc.Usage     = USAGE_IMMUTABLE;
            IndBuffDesc.BindFlags = BindFlags;
            IndBuffDesc.Size      = sizeof(Indices);
            IndBuffDesc.Mode      = Mode;
            if (Mode != BUFFER_MODE_UNDEFINED)
                IndBuffDesc.ElementByteStride = sizeof(Indices[0]);
            BufferData IBData;
            IBData.pData    = Indices.data();
            IBData.DataSize = NumIndices * sizeof(Indices[0]);
            RefCntAutoPtr<IBuffer> pBuffer;
            pDevice->CreateBuffer(IndBuffDesc, &IBData, &pBuffer);
            return pBuffer;
        }

	    struct CreatePSOInfo
	    {
            IRenderDevice* pDevice = nullptr;

		    TEXTURE_FORMAT RTVFormat = TEX_FORMAT_UNKNOWN;
            TEXTURE_FORMAT DSVFormat = TEX_FORMAT_UNKNOWN;

		    IShaderSourceInputStreamFactory* pShaderSourceFactory = nullptr;

		    const char* VSFilePath = nullptr;
            const char* PSFilePath = nullptr;

		    VERTEX_COMPONENT_FLAGS Components = VERTEX_COMPONENT_FLAG_NONE;
	    };

        RefCntAutoPtr<IPipelineState> CreatePipelineState(const CreatePSOInfo& CreateInfo)
        {
            GraphicsPipelineStateCreateInfo PSOCreateInfo;
            PipelineStateDesc&              PSODesc          = PSOCreateInfo.PSODesc;
            GraphicsPipelineDesc&           GraphicsPipeline = PSOCreateInfo.GraphicsPipeline;

            // This is a graphics pipeline
            PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

            // Pipeline state name is used by the engine to report issues.
            // It is always a good idea to give objects descriptive names.
            PSODesc.Name = "Cube PSO";

            // clang-format off
    // This tutorial will render to a single render target
    GraphicsPipeline.NumRenderTargets             = 1;
    // Set render target format which is the format of the swap chain's color buffer
    GraphicsPipeline.RTVFormats[0]                = CreateInfo.RTVFormat;
    // Set depth buffer format which is the format of the swap chain's back buffer
    GraphicsPipeline.DSVFormat                    = CreateInfo.DSVFormat;
 
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // Cull back faces
    GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;
    // Enable depth testing
    GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
            // clang-format on
            ShaderCreateInfo ShaderCI;
            // Tell the system that the shader source code is in HLSL.
            // For OpenGL, the engine will convert this into GLSL under the hood.
            ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

            // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
            ShaderCI.Desc.UseCombinedTextureSamplers = true;

            ShaderCI.pShaderSourceStreamFactory = CreateInfo.pShaderSourceFactory;
            // Create a vertex shader
            RefCntAutoPtr<IShader> pVS;
            {
                ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
                ShaderCI.EntryPoint      = "main";
                ShaderCI.Desc.Name       = "Cube VS";
                ShaderCI.FilePath        = CreateInfo.VSFilePath;
                CreateInfo.pDevice->CreateShader(ShaderCI, &pVS);
            }

            // Create a pixel shader
            RefCntAutoPtr<IShader> pPS;
            {
                ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
                ShaderCI.EntryPoint      = "main";
                ShaderCI.Desc.Name       = "Cube PS";
                ShaderCI.FilePath        = CreateInfo.PSFilePath;
                CreateInfo.pDevice->CreateShader(ShaderCI, &pPS);
            }

            InputLayoutDescX InputLayout;

            Uint32 Attrib = 0;
            if (CreateInfo.Components & VERTEX_COMPONENT_FLAG_POSITION)
                InputLayout.Add(Attrib++, 0u, 3u, VT_FLOAT32, False);

            RefCntAutoPtr<IPipelineState> pPSO;
            CreateInfo.pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &pPSO);
            return pPSO;

        }

	

	}
}


