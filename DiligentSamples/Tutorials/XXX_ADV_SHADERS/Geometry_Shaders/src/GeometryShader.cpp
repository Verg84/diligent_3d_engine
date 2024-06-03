#include"GeometryShader.hpp"
#include"MapHelper.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new GeomShader();
    }
    namespace 
    {
        struct Constants
        {
            float4x4 WorlViewProj;
            float4   ViewportSize;
            float    lineWidth;
        };
    } // namespace
    void GeomShader::LoadCubeBuffers()
    {
        // Layout of this structure matches the one we defined in the pipeline state
        struct Vertex
        {
            float3 pos;
           
        };

      
    Vertex CubeVerts[8] =
    {
        float3(-1,-1,-1),
        float3(-1,+1,-1),
        float3(+1,+1,-1),
        float3(+1,-1,-1),

        float3(-1,-1,+1),
        float3(-1,+1,+1),
        float3(+1,+1,+1),
        float3(+1,-1,+1)
    };
        // clang-format off
    Uint32 Indices[] =
    {
        2,0,1, 2,3,0,
        4,6,5, 4,7,6,
        0,7,4, 0,3,7,
        1,0,4, 1,4,5,
        1,5,2, 5,6,2,
        3,6,7, 3,2,6
    };

    BufferDesc vDesc;
    vDesc.Name="VERTEX BUFFERS";
    vDesc.Usage=USAGE_IMMUTABLE;
    vDesc.BindFlags=BIND_VERTEX_BUFFER;
    vDesc.Size=sizeof(CubeVerts);
    BufferData vData;
    vData.pData=CubeVerts;
    vData.DataSize=sizeof(CubeVerts);
    m_pDevice->CreateBuffer(vDesc,&vData,&VertexBuffer);

    BufferDesc indexDesc;
    indexDesc.Name="INEDX BUFFERS";
    indexDesc.Usage=USAGE_IMMUTABLE;
    indexDesc.BindFlags=BIND_INDEX_BUFFER;
    indexDesc.Size=sizeof(Indices);
    BufferData indexData;
    indexData.pData=Indices;
    indexData.DataSize=sizeof(Indices);
    m_pDevice->CreateBuffer(indexDesc,&indexData,&IndexBuffer);
       

    }
    void GeomShader::CreateUniformBuffer()
    {
        BufferDesc bdesc;
        bdesc.Name = "buffer constants";
        bdesc.BindFlags = BIND_UNIFORM_BUFFER;
        bdesc.Usage     = USAGE_DYNAMIC;
        bdesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        bdesc.Size           = sizeof(Constants);
        m_pDevice->CreateBuffer(bdesc, nullptr, &ShaderConstants);
    }
    void GeomShader::CreatePipelineState()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "CUBE PSO";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

        CreateUniformBuffer();

        ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        RefCntAutoPtr<IShader> pVS;
        {
            sci.Desc.Name = "cube VS";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = "cube.vsh";
            m_pDevice->CreateShader(sci, &pVS);
        }
        RefCntAutoPtr<IShader> pGS;
        {
            sci.Desc.Name = "cube GS";
            sci.Desc.ShaderType = SHADER_TYPE_GEOMETRY;
            sci.EntryPoint      = "main";
            sci.FilePath        = "cube.gsh";
            m_pDevice->CreateShader(sci, &pGS);
        }
        RefCntAutoPtr<IShader> pPS;
        {
            sci.Desc.Name = "cube PS";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "cube.psh";
            m_pDevice->CreateShader(sci, &pPS);
        }

        LayoutElement layout_elements[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False}};

        PSOCreateInfo.pVS = pVS;
        PSOCreateInfo.pGS = pGS;
        PSOCreateInfo.pPS = pPS;

        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = layout_elements;
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(layout_elements);

        PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        
        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
        VERIFY_EXPR(m_pPSO);

        m_pPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "VSConstants")->Set(ShaderConstants);
        m_pPSO->GetStaticVariableByName(SHADER_TYPE_GEOMETRY, "GSConstants")->Set(ShaderConstants);
        m_pPSO->GetStaticVariableByName(SHADER_TYPE_PIXEL, "PSConstants")->Set(ShaderConstants);

        m_pPSO->CreateShaderResourceBinding(&m_SRB, true);
        
        
    }
    void GeomShader::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);

        CreatePipelineState();
        LoadCubeBuffers();
    }
    void GeomShader::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
           // Apply rotation
    float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(ctime) * 1.0f) * float4x4::RotationX(-PI_F * 0.1f);

    // Camera is at (0, 0, -5) looking along the Z axis
    float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);

    // Get pretransform matrix that rotates the scene according the surface orientation
    auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

    // Get projection matrix adjusted to the current screen orientation
    auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

    // Compute world-view-projection matrix
    m_WorldViewProjMatrix = CubeModelTransform * View * SrfPreTransform * Proj;
    }
    void GeomShader::Render()
    {
        float ClearColor[]={0.25f,0.25f,0.5f,1.0};
        auto* pRTV=m_pSwapChain->GetCurrentBackBufferRTV();
        auto *pDSV=m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV,ClearColor,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV,CLEAR_DEPTH_FLAG,1.0,0,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        {
            MapHelper<Constants> Consts(m_pImmediateContext,ShaderConstants,MAP_WRITE,MAP_FLAG_DISCARD);
            Consts->WorlViewProj=m_WorldViewProjMatrix.Transpose();
            Consts->lineWidth=m_LineWidth;
            const auto& SCDesc   = m_pSwapChain->GetDesc();
            Consts->ViewportSize = float4(static_cast<float>(SCDesc.Width), static_cast<float>(SCDesc.Height), 1.f / static_cast<float>(SCDesc.Width), 1.f / static_cast<float>(SCDesc.Height));
        }
         IBuffer* pBuffs[] = {VertexBuffer};
        m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(IndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // Set the pipeline state
        m_pImmediateContext->SetPipelineState(m_pPSO);
        // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
        // makes sure that resources are transitioned to required states.
        m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs DrawAttrs;
        DrawAttrs.IndexType  = VT_UINT32; // Index type
        DrawAttrs.NumIndices = 36;
        // Verify the state of vertex and index buffers
        DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(DrawAttrs);
        }

    }