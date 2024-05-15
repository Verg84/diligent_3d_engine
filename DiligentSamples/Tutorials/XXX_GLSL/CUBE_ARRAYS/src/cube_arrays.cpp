#include"cube_arrays.hpp"
#include"MapHelper.hpp"
namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new CubeArrays();
    }

    void CubeArrays::LoadBuffers()
    {
        struct Vertex
        {
            float3 position;
            float4 color;
        };

        Vertex CubePositions[8] =
            {
                {float3(-1, -1, -1), float4(0.0,1.0,0.0,1.0)},
                {float3(-1, 1, -1), float4(1.0,0.0,1.0,1.0)},
                {float3(1, 1, -1), float4(0.0,1.0,0.0,1.0)},
                {float3(1, -1, -1), float4(1.0,0.0,0.0,1.0)},

                {float3(-1, -1, 1), float4(0.0,0.0,1.0,1.0)},
                {float3(-1, 1, 1),float4(0.0,1.0,1.0,1.0)},
                {float3(1, 1, 1), float4(1.0,1.0,1.0,1.0)},
                {float3(1, -1, 1), float4(1.0,0.0,1.0,1.0)}};

        Uint32 Indices[] =
            {
                2, 0, 1,
                2, 3, 0,
                4, 6, 5,
                4, 7, 6,
                0, 7, 4,
                0, 3, 7,
                1, 0, 4,
                1, 4, 5,
                1, 5, 2,
                5, 6, 2,
                3, 6, 7,
                3, 2, 6};

        BufferDesc Vdesc;
        Vdesc.Name = "Vertex Buffers";
        Vdesc.Usage=USAGE_IMMUTABLE;
        Vdesc.BindFlags = BIND_VERTEX_BUFFER;
        Vdesc.Size      = sizeof(CubePositions);

        BufferData Vdata;
        Vdata.pData = CubePositions;
        Vdata.DataSize = sizeof(CubePositions);

        m_pDevice->CreateBuffer(Vdesc, &Vdata,&VBuffers);

        BufferDesc Idesc;
        Idesc.Name = "Index Buffers";
        Idesc.BindFlags = BIND_INDEX_BUFFER;
        Idesc.Usage     = USAGE_IMMUTABLE;
        Idesc.Size      = sizeof(Indices);

        BufferData Idata;
        Idata.pData = Indices;
        Idata.DataSize = sizeof(Indices);

        m_pDevice->CreateBuffer(Idesc, &Idata, &IBuffers);
    }

    void CubeArrays::CreateUBuffer()
    {
        BufferDesc bufDesc;
        bufDesc.Name = "Uniform Buffer";
        bufDesc.Usage = USAGE_DYNAMIC;
        bufDesc.BindFlags = BIND_UNIFORM_BUFFER;
        bufDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        bufDesc.Size           = sizeof(float4x4);

        m_pDevice->CreateBuffer(bufDesc, nullptr, &VSContants);
    }
    void CubeArrays::CreatePipeline()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "PIPELINE CUBE";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

        ShaderCreateInfo sci;
        sci.SourceLanguage =SHADER_SOURCE_LANGUAGE_GLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        RefCntAutoPtr<IShader> VS;
        {
            sci.Desc.Name = "Cube Vertex Shader";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath="VS.glsl";
            m_pDevice->CreateShader(sci, &VS);
        }
        RefCntAutoPtr<IShader> PS;
        {
            sci.Desc.Name = "PIXEL SHADER CUBE";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "PS.glsl";
            m_pDevice->CreateShader(sci, &PS);
        }

        PSOCreateInfo.pVS=VS;
        PSOCreateInfo.pPS = PS;

        LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                LayoutElement{1, 0, 4, VT_FLOAT32, False}};
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(elems);
        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;

        PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        CreateUBuffer();

        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_PSO);
        m_PSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(VSContants);
        
        m_PSO->CreateShaderResourceBinding(&m_SRB, true);

 
    }
    void CubeArrays::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
        CreatePipeline();
        LoadBuffers();
    }
    void CubeArrays::Render()
    {
        float ClearColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
        auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        {
            MapHelper<float4x4> Constants(
                m_pImmediateContext,
                VSContants,
                MAP_WRITE,
                MAP_FLAG_DISCARD);
            *Constants = MVP.Transpose();
        }

        const Uint64 offset = 0;
        IBuffer*     pbuf[] = {VBuffers};
        
        m_pImmediateContext->SetVertexBuffers(
            0,
            1,
            pbuf,
            offset,
            RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
            SET_VERTEX_BUFFERS_FLAG_RESET);

        m_pImmediateContext->SetIndexBuffer(
            IBuffers,
            0,
            RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        m_pImmediateContext->SetPipelineState(m_PSO);
        m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs atr;
        atr.IndexType = VT_UINT32;
        atr.NumIndices = 36;

        m_pImmediateContext->DrawIndexed(atr);


    }
    void CubeArrays::Update(double c, double e)
    {
        SampleBase::Update(c, e);

          // Apply rotation
        float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(c) * 1.0f);

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