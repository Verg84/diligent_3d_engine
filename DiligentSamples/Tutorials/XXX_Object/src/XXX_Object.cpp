#include <vector>

#include "XXX_Object.hpp"
#include "BasicMath.hpp"
#include "TextureUtilities.h"
#include "GraphicsTypesX.hpp"


namespace Diligent
{




    RefCntAutoPtr<IBuffer> Object::CreateVertexBuffer(IRenderDevice* pDevice, VERTEX_FLAGS VertexFlags, BIND_FLAGS BindFlags, BUFFER_MODE Mode)
    {
        VERIFY_EXPR(VertexFlags != NONE_FLAG);
        const Uint32 TotalVertexComponents =
            ((VertexFlags & POS_FLAG) ? 3 : 0) +
            ((VertexFlags & NORMAL_FLAG) ? 3 : 0) +
            ((VertexFlags & TEXCOORD_FLAG) ? 2 : 0);
        std::vector<float> VertexData(size_t{TotalVertexComponents}*NV);
        auto it=VertexData.begin();
        for (Uint32 v = 0; v < NV; ++v)
        {
            if (VertexFlags & POS_FLAG)
            {
                const auto& Pos{Positions[v]};
                *(it++) = Pos.x;
                *(it++) = Pos.y;
                *(it++) = Pos.z;

            }
            if (VertexFlags & NORMAL_FLAG)
            {
                const auto& N{Normals[v]};
                *(it++) = N.x;
                *(it++) = N.y;
                *(it++) = N.z;
            }
            if (VertexFlags & TEXCOORD_FLAG)
            {
                const auto& UV{TexCoords[v]};
                *(it++) = UV.x;
                *(it++) = UV.y;
            }
        }
        VERIFY_EXPR(it == VertexData.end());

        BufferDesc vBufferDesc;
        vBufferDesc.Name = "Cube Vertex Buffer";
        vBufferDesc.Usage = USAGE_IMMUTABLE;
        vBufferDesc.BindFlags = BIND_VERTEX_BUFFER;
        vBufferDesc.Size      = static_cast<Uint64>(VertexData.size()*sizeof(VertexData[0]));
        vBufferDesc.Mode=Mode;
        if (Mode != BUFFER_MODE_UNDEFINED)
        {
            vBufferDesc.ElementByteStride = TotalVertexComponents * sizeof(VertexData[0]);
        }
        BufferData vBufferData;
        vBufferData.pData = VertexData.data();
        vBufferData.DataSize = VertexData.size();
        RefCntAutoPtr<IBuffer> vertex_buffer;
        pDevice->CreateBuffer(vBufferDesc, &vBufferData, &vertex_buffer);
    }
    RefCntAutoPtr<IBuffer> Object::CreateIndexBuffer(IRenderDevice* pDevice, BIND_FLAGS Bind_Flags, BUFFER_MODE Mode)
    {
        BufferDesc IndexBufferDesc;
        IndexBufferDesc.Name = "index buffer";
        IndexBufferDesc.BindFlags = Bind_Flags;
        IndexBufferDesc.Usage     =USAGE_IMMUTABLE;
        IndexBufferDesc.Size      = sizeof(Indices);
        IndexBufferDesc.Mode      = Mode;
        if (Mode != BUFFER_MODE_UNDEFINED)
            IndexBufferDesc.ElementByteStride = Indices[0];
        RefCntAutoPtr<IBuffer> index_buffer;
        BufferData             IndexBufferData;
        IndexBufferData.pData = Indices.data();
        IndexBufferData.DataSize = Indices.size();
        pDevice->CreateBuffer(IndexBufferDesc, &IndexBufferData, &index_buffer);
    }
    RefCntAutoPtr<ITexture>Object::LoadTexture(IRenderDevice* pDevice, const char* path)
    {
        TextureLoadInfo loadinfo;
        loadinfo.IsSRGB = true;
        RefCntAutoPtr<ITexture> ptex;
        CreateTextureFromFile(path, loadinfo, pDevice, &ptex);
        return ptex;
    }

    RefCntAutoPtr<IPipelineState> Object::CreatePipelineState(const CreatePSOInfo& CreateInfo)
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PipelineStateDesc&              PSODesc = PSOCreateInfo.PSODesc;
        PipelineResourceLayoutDesc&     ResourceLayout = PSODesc.ResourceLayout;
        GraphicsPipelineDesc&           GraphicsPipeline = PSOCreateInfo.GraphicsPipeline;

        PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
        PSODesc.Name         = "PIPELINE STATE OBJECT";
        GraphicsPipeline.NumRenderTargets = 1;
        GraphicsPipeline.RTVFormats[0]=CreateInfo.pRTV;
        GraphicsPipeline.DSVFormat        = CreateInfo.pDSV;
        GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        GraphicsPipeline.SmplDesc.Count    = CreateInfo.SampleCount;
        GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

        ShaderCreateInfo ShaderCI;
        ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        ShaderCI.Desc.UseCombinedTextureSamplers=true;

       
        ShaderCI.pShaderSourceStreamFactory = CreateInfo.pShaderSourceFactory;

        RefCntAutoPtr<IShader> pVS;
        {
            ShaderCI.Desc.Name = "vertex shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = CreateInfo.VSFilePath;
            CreateInfo.pDevice->CreateShader(ShaderCI, &pVS);
        }

        RefCntAutoPtr<IShader> pPS;
        {
            ShaderCI.Desc.Name = "pixel shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = CreateInfo.PSFilePath;
            CreateInfo.pDevice->CreateShader(ShaderCI, &pPS);
        }

        PSOCreateInfo.pVS = pVS;
        PSOCreateInfo.pPS = pPS;

        ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        ShaderResourceVariableDesc Vars[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
        ResourceLayout.Variables = Vars;
        ResourceLayout.NumVariables = _countof(Vars);

        SamplerDesc sampler =
            {
                FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
                TEXTURE_ADDRESS_CLAMP,
                TEXTURE_ADDRESS_CLAMP,
                TEXTURE_ADDRESS_CLAMP};
        ImmutableSamplerDesc immutables[]=
        {
            {SHADER_TYPE_PIXEL, "g_Texture",sampler}
        };
        ResourceLayout.ImmutableSamplers = immutables;
        ResourceLayout.NumImmutableSamplers = _countof(immutables);

        RefCntAutoPtr<IPipelineState> pPSO;
        CreateInfo.pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &pPSO);
        return pPSO;

    }
    
}
