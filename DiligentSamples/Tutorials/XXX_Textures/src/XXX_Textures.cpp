#include"XXX_Textures.hpp"
#include "MapHelper.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"
namespace Diligent
{
	SampleBase* CreateSample()
	{
    
	return new Texturing();
	}
    void Texturing::CreateVertexBuffer()
    {
        struct Vertex
        {
            float3 pos;
            float2 uv;
        };

       
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
    BufferDesc vd;
    vd.Name = "Vertex Buffers";
    vd.BindFlags = BIND_VERTEX_BUFFER;
    vd.Usage     = USAGE_IMMUTABLE;
    vd.Size      = sizeof(CubeVerts);
    BufferData vb;
    vb.pData = CubeVerts;
    vb.DataSize = sizeof(CubeVerts);
    m_pDevice->CreateBuffer(vd, &vb, &vertex_buf_handler);
    }
    void Texturing::CreateIndexBuffer()
    {
        Uint32 Indices[] =
            {
                2, 0, 1, 2, 3, 0,
                4, 6, 5, 4, 7, 6,
                8, 10, 9, 8, 11, 10,
                12, 14, 13, 12, 15, 14,
                16, 18, 17, 16, 19, 18,
                20, 21, 22, 20, 22, 23};
        BufferDesc id;
        id.Name = "Vertex Indices Buffer";
        id.Usage = USAGE_IMMUTABLE;
        id.BindFlags = BIND_INDEX_BUFFER;
        id.Size      = sizeof(Indices);
        BufferData ib;
        ib.pData = Indices;
        ib.DataSize = sizeof(Indices);
        m_pDevice->CreateBuffer(id, &ib, &index_buf_handler);

    }
    void Texturing::CreateUniformBuffer()
    {
        BufferDesc bufDesc;
        bufDesc.Name = "vertex shader uniform constants";
        bufDesc.BindFlags = BIND_UNIFORM_BUFFER;
        bufDesc.Usage     = USAGE_DYNAMIC;
        bufDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        bufDesc.Size           = sizeof(float4x4);
        m_pDevice->CreateBuffer(bufDesc,nullptr,&uniform_buf_handler);
    }
    void Texturing::LoadShaders(const Char* vsrc, const Char* psrc)
    {
        ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;
        {
            sci.Desc.Name = "Vertex Shader";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = vsrc;
            m_pDevice->CreateShader(sci, &vertex_shader);
        }
        {
            sci.Desc.Name = "Pixel Shader";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = psrc;
            m_pDevice->CreateShader(sci, &pixel_shader);
        }

    }
    
    void Texturing::LoadTexture()
    {
        TextureLoadInfo texloadinfo;
        texloadinfo.IsSRGB = true;
        RefCntAutoPtr<ITexture> tex;
        CreateTextureFromFile("DGLogo.png", texloadinfo, m_pDevice, &tex);
        texture_handler = tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
        resource_handler->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(texture_handler);
    }
    
	void Texturing::CreatePipeState()
	{
        GraphicsPipelineStateCreateInfo ps;
        ps.PSODesc.Name = "Texturing Pipeline";
        ps.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
        GraphicsPipelineDesc& graphs = ps.GraphicsPipeline;
        graphs.NumRenderTargets      = 1;
        graphs.RTVFormats[0]         = m_pSwapChain->GetDesc().ColorBufferFormat;
        graphs.DSVFormat             = m_pSwapChain->GetDesc().DepthBufferFormat;
        graphs.PrimitiveTopology     = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        DepthStencilStateDesc& depth = graphs.DepthStencilDesc;
        depth.DepthEnable            = true;
        RasterizerStateDesc& rast    = graphs.RasterizerDesc;
        rast.CullMode                = CULL_MODE_BACK;
        InputLayoutDesc& layout      = ps.GraphicsPipeline.InputLayout;
        LayoutElement    elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                LayoutElement{1, 0, 2, VT_FLOAT32, False}};
        layout.LayoutElements = elems;
        layout.NumElements    = _countof(elems);
        ShaderResourceVariableDesc vars[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
        ps.PSODesc.ResourceLayout.Variables = vars;
        ps.PSODesc.ResourceLayout.NumVariables = _countof(vars);
        SamplerDesc sampler =
            {
                FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
                TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};
        ImmutableSamplerDesc im[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture", sampler}};
        ps.PSODesc.ResourceLayout.ImmutableSamplers = im;
        ps.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(im);
        LoadShaders("textures_vertex_shader_source.vsh", "textures_pixel_shader_source.psh");
        ps.pVS = vertex_shader;
        ps.pPS = pixel_shader;
        CreateUniformBuffer();
        ps.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        m_pDevice->CreatePipelineState(ps, &pipe_handler);
        pipe_handler->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(uniform_buf_handler);

        pipe_handler->CreateShaderResourceBinding(&resource_handler, true);
		
	}
	void Texturing::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        CreatePipeState();
        CreateVertexBuffer();
        CreateIndexBuffer();
        LoadTexture();
	}
	void Texturing::Render()
	{
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        const float ClearColor[] = {0.1f, 0.1f, 0.5f, 1.0f};
        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        {
            MapHelper<float4x4> Constants(m_pImmediateContext, uniform_buf_handler, MAP_WRITE, MAP_FLAG_DISCARD);
            *Constants = MVP.Transpose();
        }

        const Uint64 offset = 0;
        IBuffer*     buf[]  = {vertex_buf_handler};
        m_pImmediateContext->SetVertexBuffers(0, 1, buf, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(index_buf_handler, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        m_pImmediateContext->SetPipelineState(pipe_handler);
        m_pImmediateContext->CommitShaderResources(resource_handler,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
        DrawAttrs.IndexType  = VT_UINT32; // Index type
        DrawAttrs.NumIndices = 36;
        // Verify the state of vertex and index buffers
        DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(DrawAttrs);

	}
    void Texturing::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
        float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(ctime) * 1.0f) * float4x4::RotationX(-PI_F * 0.1f);

        // Camera is at (0, 0, -5) looking along the Z axis
        float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);

        // Get pretransform matrix that rotates the scene according the surface orientation
        auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

        // Get projection matrix adjusted to the current screen orientation
        auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

        // Compute world-view-projection matrix
        MVP = CubeModelTransform * View * SrfPreTransform * Proj;
	}

	
}