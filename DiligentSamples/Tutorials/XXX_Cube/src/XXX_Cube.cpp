#include"XXX_Cube.hpp"
#include"MapHelper.hpp"
namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new XXX_Cube();
	}

    void XXX_Cube::CreateVertexBuffers()
    {
        struct Vertex
        {
            float3 pos;
            float4 color;
        };

        Vertex CubeVerts[8] =
            {
                {float3(-1, -1, -1), float4(1, 0, 0, 1)},
                {float3(-1, +1, -1), float4(0, 1, 0, 1)},
                {float3(+1, +1, -1), float4(0, 0, 1, 1)},
                {float3(+1, -1, -1), float4(1, 1, 1, 1)},

                {float3(-1, -1, +1), float4(1, 1, 0, 1)},
                {float3(-1, +1, +1), float4(0, 1, 1, 1)},
                {float3(+1, +1, +1), float4(1, 0, 1, 1)},
                {float3(+1, -1, +1), float4(0.2f, 0.2f, 0.2f, 1)},
            };
        BufferDesc vbd;
        vbd.Name = "vertex positions";
        vbd.BindFlags = BIND_VERTEX_BUFFER;
        vbd.Usage     = USAGE_IMMUTABLE;
        vbd.Size      = sizeof(CubeVerts);
        BufferData vd;
        vd.pData = CubeVerts;
        vd.DataSize = sizeof(CubeVerts);
        m_pDevice->CreateBuffer(vbd, &vd, &vertex_buffer);
    }
    void XXX_Cube::CreateIndexBuffers()
    {
        Uint32 Indices[] =
            {
                2, 0, 1, 2, 3, 0,
                4, 6, 5, 4, 7, 6,
                0, 7, 4, 0, 3, 7,
                1, 0, 4, 1, 4, 5,
                1, 5, 2, 5, 6, 2,
                3, 6, 7, 3, 2, 6};
        BufferDesc ibd;
        ibd.Name = "cube indices";
        ibd.BindFlags = BIND_INDEX_BUFFER;
        ibd.Usage     = USAGE_IMMUTABLE;
        ibd.Size      = sizeof(Indices);
        BufferData id;
        id.pData = Indices;
        id.DataSize = sizeof(Indices);
        m_pDevice->CreateBuffer(ibd, &id, &index_bufer);
    }

    void XXX_Cube::CreateUniformBuffer()
    {
        BufferDesc cbd;
        cbd.Name = "vertex shader constants buffer";
        cbd.BindFlags = BIND_UNIFORM_BUFFER;
        cbd.Usage     = USAGE_DYNAMIC;
        cbd.CPUAccessFlags = CPU_ACCESS_WRITE;
        cbd.Size           = sizeof(float4x4);
        m_pDevice->CreateBuffer(cbd, nullptr, &constants_buffer);
    }
    void XXX_Cube::LoadShaders(const Char* vSrc, const Char *pSrc)
    {
        ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;
        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;
        {
            sci.Desc.Name = "\t...vertex shader...\n";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = vSrc;
            m_pDevice->CreateShader(sci, &vertex_shader);
        }
        {
            sci.Desc.Name       = "\t...pixel shader...\n";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = pSrc;
            m_pDevice->CreateShader(sci, &pixel_shader);


        }
    }
    void XXX_Cube::CreatePipelineState()
    {
        GraphicsPipelineStateCreateInfo PSOinfo;
        PSOinfo.PSODesc.Name = "Graphics Pipeline";
        PSOinfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
        
        // ... graphics configuration
        GraphicsPipelineDesc& graphicsDesc = PSOinfo.GraphicsPipeline;
        graphicsDesc.NumRenderTargets      = 1;
        graphicsDesc.RTVFormats[0]         = m_pSwapChain->GetDesc().ColorBufferFormat;
        graphicsDesc.DSVFormat             = m_pSwapChain->GetDesc().DepthBufferFormat;
        graphicsDesc.PrimitiveTopology     = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        // ... stencil-depth state configuration
        DepthStencilStateDesc& depstencil = PSOinfo.GraphicsPipeline.DepthStencilDesc;
        depstencil.DepthEnable            = true;
        // ... rasterizer state setup
        RasterizerStateDesc& raster = PSOinfo.GraphicsPipeline.RasterizerDesc;
        raster.CullMode             = CULL_MODE_BACK;
        // ... input layout
        InputLayoutDesc& layoutIn = PSOinfo.GraphicsPipeline.InputLayout;
        LayoutElement    elems[]=
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                LayoutElement{1, 0, 4, VT_FLOAT32, False}};
        layoutIn.LayoutElements = elems;
        layoutIn.NumElements    = _countof(elems);

        LoadShaders("cube.vsh", "cube.psh");
        PSOinfo.pVS = vertex_shader;
        PSOinfo.pPS = pixel_shader;
        CreateUniformBuffer();
      
        m_pDevice->CreateGraphicsPipelineState(PSOinfo, &pipeline_state);

        //... handling resources
        PSOinfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        //... bound STATIC shader resources to the buffer directly to the pipeline
        pipeline_state->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(constants_buffer);
        //... create Shader Resource Bind Object
        pipeline_state->CreateShaderResourceBinding(&resource_binding, true);

    }
	
	void XXX_Cube::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
        CreatePipelineState();
        CreateVertexBuffers();
        CreateIndexBuffers();
	}
    void XXX_Cube::Render()
    {
        float4 ClearColor = {0.0f, 0.0f, 0.3f, 1.0f};
        auto*  pRTV       = m_pSwapChain->GetCurrentBackBufferRTV();
        auto*  pDSV       = m_pSwapChain->GetDepthBufferDSV();
        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor.Data(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        {
            MapHelper<float4x4> vc(m_pImmediateContext,constants_buffer, MAP_WRITE, MAP_FLAG_DISCARD);
            *vc = MVP.Transpose();
        }
        const Uint64 offset = 0;
        IBuffer*     pbuff[] = {vertex_buffer};
        m_pImmediateContext->SetVertexBuffers(0, 1, pbuff, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(index_bufer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // ... set pipeline-state
        m_pImmediateContext->SetPipelineState(pipeline_state);
        m_pImmediateContext->CommitShaderResources(resource_binding, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs datr;
        datr.IndexType = VT_UINT32;
        datr.NumIndices = 36;
        datr.Flags      = DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(datr);

	}
    void XXX_Cube::Update(double CurTime, double ElapsTime)
    {
        SampleBase::Update(CurTime, ElapsTime);

        float4x4 Model = float4x4::Translation(0.0f, 0.0f, 0.0f);
        float4x4 View  = float4x4::Translation(0.0f, 0.0f, 8.0f);
        float4x4 SurfacePreTrans = GetSurfacePretransformMatrix(float3(0, 0, 1));
        auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);
        MVP=Model*View*SurfacePreTrans*Proj;
    }

    }