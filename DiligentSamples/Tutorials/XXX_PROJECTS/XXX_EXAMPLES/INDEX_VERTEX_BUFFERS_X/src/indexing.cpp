#include"Indexing.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Indexing();
    }
    void Indexing::CreateBuffers()
    {
        struct Vertex
        {
            float3 pos;
        };
        Vertex CubeVerts[] =
            {
                {float3{-1, -1, 0}},
                {float3{0, -1, 0}},
                {float3{1, 0, 0}}
              
            };
     
        BufferDesc vdesk;
        vdesk.Name = "Vertex Buffers";
        vdesk.BindFlags = BIND_VERTEX_BUFFER;
        vdesk.Usage     = USAGE_IMMUTABLE;
        vdesk.Size      = sizeof(CubeVerts);
        BufferData vdata;
        vdata.pData = CubeVerts;
        vdata.DataSize = sizeof(CubeVerts);
        m_pDevice->CreateBuffer(vdesk, &vdata, &vertex_buffer);

       

        
    }
    void Indexing::CreatePipeline()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "Pieline Indexing";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

       // SHADERS
        ShaderCreateInfo sci;
        sci.Desc.UseCombinedTextureSamplers=true;
        sci.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_GLSL;
        
        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        RefCntAutoPtr<IShader> VS;
        {
            sci.Desc.Name = "Vertex Shader";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = "VS.glsl";
            m_pDevice->CreateShader(sci, &VS);
        }
        RefCntAutoPtr<IShader> PS;
        {
            sci.Desc.Name = "Pixel Shader";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "PS.glsl";
            m_pDevice->CreateShader(sci, &PS);
        }

       

        LayoutElement vertex_attributes[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
               };
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(vertex_attributes);
        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = vertex_attributes;
        PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType  = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

        PSOCreateInfo.pVS = VS;
        PSOCreateInfo.pPS = PS;


        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

        m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);


    }
	void Indexing::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        CreatePipeline();
        CreateBuffers();

    }
    void Indexing::Update(double c,double e)
    {
        SampleBase::Update(c, e);
    }
    void Indexing::Render()
    {
        float ClearColor[] = {0.35f, 0.35f, 0.35f, 1.0f};
        auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG_NONE, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        const Uint64 offset = 0;
        IBuffer*     pBuffs[] = {vertex_buffer};
        m_pImmediateContext->SetVertexBuffers(0,1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
       // m_pImmediateContext->SetIndexBuffer(index_buffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        m_pImmediateContext->SetPipelineState(m_pPSO);
        m_pImmediateContext->CommitShaderResources(m_pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawAttribs DrawAttrs;     // This is an indexed draw call
        //DrawAttrs.IndexType  = VT_UINT32; // Index type
       // DrawAttrs.NumIndices = 3;
        // Verify the state of vertex and index buffers
        //DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        DrawAttrs.NumVertices = 3;
        m_pImmediateContext->Draw(DrawAttrs);

    }

    }
