#include"GeometryShader001.hpp"
namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new GeomShader_PassThrough();
    }
    void GeomShader_PassThrough::LoadBuffers()
    {
        struct Vertex
        {
            float3 pos;
        };

        const Vertex positions[3] =
            {
                {float3(0.0, 0.0, 0.0)},
                {float3(1.0, 0.0, 0.0)},
                {float3(0.0, 1.0, 0.0)}};
        BufferDesc vDesc;
        vDesc.Usage = USAGE_IMMUTABLE;
        vDesc.BindFlags = BIND_VERTEX_BUFFER;
        vDesc.Name      = "VERTEX BUFFER";
        vDesc.Size      = sizeof(positions);
        BufferData vData;
        vData.pData = positions;
        vData.DataSize = sizeof(positions);
        m_pDevice->CreateBuffer(vDesc, &vData, &VertexBuffer);
    }
    void GeomShader_PassThrough::CreatePipelineState()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "Geometry Pipeline";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_NONE;

        ShaderCreateInfo sci;
        sci.Desc.UseCombinedTextureSamplers = true;
        sci.SourceLanguage=SHADER_SOURCE_LANGUAGE_HLSL;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        RefCntAutoPtr<IShader> VS;
        {
            sci.Desc.Name = "VS";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = "cube.vsh";
            m_pDevice->CreateShader(sci, &VS);
        }
        RefCntAutoPtr<IShader> GS;
        {
            sci.Desc.Name = "GS";
            sci.Desc.ShaderType = SHADER_TYPE_GEOMETRY;
            sci.EntryPoint      = "main";
            sci.FilePath        = "cube.gsh";
            m_pDevice->CreateShader(sci, &GS);
        }
        RefCntAutoPtr<IShader> PS;
        {
            sci.Desc.Name = "PS";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "cube.psh";
            m_pDevice->CreateShader(sci, &PS);
        }

        PSOCreateInfo.pVS = VS;
        PSOCreateInfo.pGS = GS;
        PSOCreateInfo.pPS = PS;

        LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False}};
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(elems);
        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;

        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_PSO);
        m_PSO->CreateShaderResourceBinding(&m_SRB,true);
    }
    void GeomShader_PassThrough::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
        CreatePipelineState();
        LoadBuffers();
    }
    void GeomShader_PassThrough::Render()
    {
        float ClearColor[] = {0.2f, .2f, 0.5f, 1.0f};
        auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        const Uint64 offset = 0;
        IBuffer*     pBuffs[] = {VertexBuffer};
        m_pImmediateContext->SetVertexBuffers(0,1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

        m_pImmediateContext->SetPipelineState(m_PSO);
        m_pImmediateContext->CommitShaderResources(m_SRB,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawAttribs dra;
        dra.NumVertices = 3;
        dra.Flags=DRAW_FLAG_VERIFY_ALL;

        m_pImmediateContext->Draw(dra);
    }
    void GeomShader_PassThrough::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }

    }