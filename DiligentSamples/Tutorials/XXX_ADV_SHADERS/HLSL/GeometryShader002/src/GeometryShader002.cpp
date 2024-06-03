#include"GeometryShader002.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new GeomShader002();
    }
    void GeomShader002::LoadBuffers()
    {
        struct Vertex
        {
            float4 pos;
        };

        Vertex positions[9] =
            {
                {float4(-2.0f, 0.0f, 0.0f, 1.0f)},
                {float4(-1.0f, 0.0f, 0.0f, 1.0f)},
                {float4(0.0f, -1.0f, 0.0f, 1.0f)},
                {float4(0.0f, 0.0f, 0.0f, 1.0f)},
                {float4(0.2f, 0.0f, 0.0f, 1.0f)},
                {float4(0.1f, 0.2f, 0.0f, 1.0f)},
                {float4(0.3f, 0.0f, 0.0f, 1.0f)},
                {float4(0.6f, 0.0f, 0.0f, 1.0f)},
                {float4(0.4f, 0.4f, 0.0f, 1.0f)}
        };
        BufferDesc vdesc;
        vdesc.Name = "Vertex buffer";
        vdesc.BindFlags = BIND_VERTEX_BUFFER;
        vdesc.Usage     = USAGE_IMMUTABLE;
        vdesc.Size      = sizeof(positions);
        BufferData vdata;
        vdata.pData = positions;
        vdata.DataSize = sizeof(positions);
        m_pDevice->CreateBuffer(vdesc, &vdata, &Vertex_Buffer);
    }
    void GeomShader002::CreatePipelineState()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "GEOM PIPELINE";
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
                LayoutElement{0, 0, 4, VT_FLOAT32, False}};
        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);

        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_PSO);

        
    }
    void GeomShader002::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
        CreatePipelineState();
        LoadBuffers();
    }
    void GeomShader002::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }
    void GeomShader002::Render()
    {
        float ClearColor[] = {0.1f, 0.1f, 0.0f, 1.0f};
        auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        const Uint64 offset   = 0;
        IBuffer*     pBuffs[] = {Vertex_Buffer};
        m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

        m_pImmediateContext->SetPipelineState(m_PSO);

        DrawAttribs dra;
        dra.NumVertices = 9;
        dra.Flags       = DRAW_FLAG_VERIFY_ALL;

        m_pImmediateContext->Draw(dra);
    }

    }