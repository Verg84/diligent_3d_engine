#include "XXX_Functions.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"
namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Functions();
    }
    void Functions::LoadVertices()
    {
        struct Vertex
        {
            float3 pos;
            float2 uv;
        };

        // Cube vertices

        //      (-1,+1,+1)________________(+1,+1,+1)
        //               /|              /|
        //              / |             / |
        //             /  |            /  |
        //            /   |           /   |
        //(-1,-1,+1) /____|__________/(+1,-1,+1)
        //           |    |__________|____|
        //           |   /(-1,+1,-1) |    /(+1,+1,-1)
        //           |  /            |   /
        //           | /             |  /
        //           |/              | /
        //           /_______________|/
        //        (-1,-1,-1)       (+1,-1,-1)
        //

        // clang-format off
    // This time we have to duplicate verices because texture coordinates cannot
    // be shared
    Vertex CubeVerts[] =
    {
        {float3(-1,-1,-1), float2(0,1)},
        {float3(-1,+1,-1), float2(0,0)},
        {float3(+1,+1,-1), float2(1,0)},
        {float3(+1,-1,-1), float2(1,1)},

        {float3(-1,-1,-1), float2(0,1)},
        {float3(-1,-1,+1), float2(0,0)},
        {float3(+1,-1,+1), float2(1,0)},
        {float3(+1,-1,-1), float2(1,1)},

        {float3(+1,-1,-1), float2(0,1)},
        {float3(+1,-1,+1), float2(1,1)},
        {float3(+1,+1,+1), float2(1,0)},
        {float3(+1,+1,-1), float2(0,0)},

        {float3(+1,+1,-1), float2(0,1)},
        {float3(+1,+1,+1), float2(0,0)},
        {float3(-1,+1,+1), float2(1,0)},
        {float3(-1,+1,-1), float2(1,1)},

        {float3(-1,+1,-1), float2(1,0)},
        {float3(-1,+1,+1), float2(0,0)},
        {float3(-1,-1,+1), float2(0,1)},
        {float3(-1,-1,-1), float2(1,1)},

        {float3(-1,-1,+1), float2(1,1)},
        {float3(+1,-1,+1), float2(0,1)},
        {float3(+1,+1,+1), float2(0,0)},
        {float3(-1,+1,+1), float2(1,0)}
    };
        BufferDesc vertsDesc;
        vertsDesc.Name = "Vertex Buffer";
        vertsDesc.BindFlags = BIND_VERTEX_BUFFER;
        vertsDesc.Usage =USAGE_IMMUTABLE;
        vertsDesc.Size = sizeof(CubeVerts);
        BufferData vertData;
        vertData.pData=CubeVerts;
        vertData.DataSize=sizeof(CubeVerts);
        m_pDevice->CreateBuffer(vertsDesc,&vertData,&positions_buffer);

    }
    void Functions::Draw()
    {
        DrawIndexedAttribs datrs;
        datrs.IndexType=VT_UINT32;
        datrs.NumIndices=36;
        datrs.Flags=DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(datrs);
    }
    void Functions::LoadTexture()
    {
        TextureLoadInfo loadInfo;
        loadInfo.IsSRGB = true;
        RefCntAutoPtr<ITexture> Tex;
        CreateTextureFromFile("DGLogo.png", loadInfo, m_pDevice, &Tex);
    // Get shader resource view from the texture
        texture_handler = Tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

    // Set texture SRV in the SRB
        resouce_handler->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(texture_handler);
    }
    void Functions::BindBuffers()
    {
        const Uint64 offset=0;
        IBuffer *pbuffs[]={positions_buffer};
        m_pImmediateContext->SetVertexBuffers(0,1,pbuffs,&offset,RESOURCE_STATE_TRANSITION_MODE_TRANSITION,SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(index_buffer,0,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }
    void Functions::LoadIndices()
    {
        Uint32 Indices[] =
    {
        2,0,1,    2,3,0,
        4,6,5,    4,7,6,
        8,10,9,   8,11,10,
        12,14,13, 12,15,14,
        16,18,17, 16,19,18,
        20,21,22, 20,22,23
    };
        BufferDesc indexDesc;
        indexDesc.Name="Index Buffer";
        indexDesc.BindFlags=BIND_INDEX_BUFFER;
        indexDesc.Usage=USAGE_IMMUTABLE;
        indexDesc.Size=sizeof(Indices);
        BufferData indexData;
        indexData.pData=Indices;
        indexData.DataSize=sizeof(Indices);
        m_pDevice->CreateBuffer(indexDesc,&indexData,&index_buffer);
    }
    void Functions::CreateUniformBuffer()
    {
        BufferDesc ubDesc;
        ubDesc.Name = "uniform buffer";
        ubDesc.BindFlags=BIND_UNIFORM_BUFFER;
        ubDesc.Usage     = USAGE_DYNAMIC;
        ubDesc.CPUAccessFlags=CPU_ACCESS_WRITE;
        ubDesc.Size =sizeof(float4x4);
        m_pDevice->CreateBuffer(ubDesc, nullptr, &uniform_buffer);
    }
    void Functions::LoadShaders(const Char* vsrc, const Char* psrc)
    {
        ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;
        RefCntAutoPtr<IShaderSourceInputStreamFactory> psf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &psf);
        sci.pShaderSourceStreamFactory = psf;
        {
            sci.Desc.Name = "vertex shader";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = vsrc;
            m_pDevice->CreateShader(sci, &vertex_shader);
        }
        {
            sci.Desc.Name = "pixel shader";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = psrc;
            m_pDevice->CreateShader(sci, &pixel_shader);
        }
        

    }
    void Functions::PipelineStateInit()
    {
        GraphicsPipelineStateCreateInfo psi;
        psi.PSODesc.Name         = "Graphics Pipeline";
        psi.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        GraphicsPipelineDesc& graphs = psi.GraphicsPipeline;
        graphs.NumRenderTargets      = 1;
        graphs.RTVFormats[0]         = m_pSwapChain->GetDesc().ColorBufferFormat;
        graphs.DSVFormat             = m_pSwapChain->GetDesc().DepthBufferFormat;
        graphs.PrimitiveTopology     = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        DepthStencilStateDesc& depth = graphs.DepthStencilDesc;
        depth.DepthEnable            = true;
        RasterizerStateDesc& rast    = graphs.RasterizerDesc;
        rast.CullMode                = CULL_MODE_BACK;

        LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                LayoutElement{1, 0, 2, VT_FLOAT32, False}};
        InputLayoutDesc& layout = graphs.InputLayout;
        layout.LayoutElements   = elems;
        layout.NumElements      = _countof(elems);

       
        psi.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        PipelineResourceLayoutDesc res = psi.PSODesc.ResourceLayout;
        ShaderResourceVariableDesc vars[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture",SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}
            };
        SamplerDesc sampler
        {
            FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};
        ImmutableSamplerDesc immut[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture",sampler}
            };
        res.Variables = vars;
        res.NumVariables = _countof(vars);
        res.ImmutableSamplers = immut;
        res.NumImmutableSamplers = _countof(immut);

        LoadShaders("vertex_shader_source.vsh", "pixel_shader_source.psh");
        
        psi.pVS = vertex_shader;
        psi.pPS = pixel_shader;
        CreateUniformBuffer();
        m_pDevice->CreateGraphicsPipelineState(psi,&pipeline_handler);
        pipeline_handler->GetStaticVariableByName(SHADER_TYPE_VERTEX,"Constants")->Set(uniform_buffer);

        pipeline_handler->CreateShaderResourceBinding(&resouce_handler,true);

    }
    void Functions::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
        PipelineStateInit();
        LoadVertices();
        LoadIndices();
        LoadTexture();
    }
    void Functions::SetUniformMatrix()
    {
        {
            MapHelper<float4x4> Mat(m_pImmediateContext,uniform_buffer,MAP_WRITE,MAP_FLAG_DISCARD);
            *Mat=MVP.Transpose();
        }
    }
    void Functions::Clear()
    {
        const float ClearColor[]={0.2f,0.9f,0.1f,1.0f};
        auto* pRTV=m_pSwapChain->GetCurrentBackBufferRTV();
        auto *pDSV=m_pSwapChain->GetDepthBufferDSV();
        m_pImmediateContext->ClearRenderTarget(pRTV,ClearColor,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV,CLEAR_DEPTH_FLAG,1.0f,0,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }
    void Functions::Render()
    {
        Clear();
        SetUniformMatrix();
        BindBuffers();
        m_pImmediateContext->SetPipelineState(pipeline_handler);
        m_pImmediateContext->CommitShaderResources(resouce_handler,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        Draw();
    }
    void Functions::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
        float4x4 Model=float4x4::RotationY(static_cast<float>(ctime))*float4x4::RotationX(-PI_F*0.1f);
         // Camera is at (0, 0, -5) looking along the Z axis
         float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);

    // Get pretransform matrix that rotates the scene according the surface orientation
        auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

    // Get projection matrix adjusted to the current screen orientation
        auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

    // Compute world-view-projection matrix
        MVP = Model * View * SrfPreTransform * Proj;
    }

    }