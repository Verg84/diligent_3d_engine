#include"Layout_Elements.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Layout_Elements();
    }

    void Layout_Elements::CreateVertexBuffer()
    {
        struct Vertex
        {
            float4 Position;
        };

        Vertex Triangle_Positions[] =
            {
                {float4(0.0f, 0.0f, 0.0f, 1.0f)},
                {float4(1.0f, 0.0f, 0.0f, 1.0f)},
                {float4(0.0, 1.0f, 0.0f, 1.0f)}};
        BufferDesc vdesc;
        vdesc.BindFlags=BIND_VERTEX_BUFFER;
        vdesc.Usage     = USAGE_IMMUTABLE;
        vdesc.Size     = sizeof(Triangle_Positions);
        BufferData vdata;
        vdata.pData = Triangle_Positions;
        vdata.DataSize = sizeof(Triangle_Positions);
        m_pDevice->CreateBuffer(vdesc, &vdata, &vertex_buffer);
    }
	void Layout_Elements::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);	

		GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "Pipeline with Layout Elements";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

		PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

		PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode=CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;

		ShaderCreateInfo sci;
        sci.SourceLanguage= SHADER_SOURCE_LANGUAGE_GLSL;
        sci.Desc.UseCombinedTextureSamplers = true;
		
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

        PSOCreateInfo.pVS = VS;
        PSOCreateInfo.pPS = PS;

        LayoutElement vertex_attributes[] =
            {
                LayoutElement{0, 0, 4, VT_FLOAT32, False}};
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(vertex_attributes);
        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = vertex_attributes;

        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);


        m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);

        CreateVertexBuffer();

	}
    void Layout_Elements::Render()
    {
        float ClearColor[] = {0.35f, 0.35f, 0.35f, 1.0f};
        auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();
        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG_NONE, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        const Uint64 offset = 0;
        IBuffer*     buffer[] = {vertex_buffer};
        m_pImmediateContext->SetVertexBuffers(
            0,
            1,
            buffer,
            &offset,
            RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
            SET_VERTEX_BUFFERS_FLAG_RESET);
        
        m_pImmediateContext->SetPipelineState(m_pPSO);
        m_pImmediateContext->CommitShaderResources(m_pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawAttribs draw;
        draw.NumVertices = 3;
        m_pImmediateContext->Draw(draw);
	}

	void Layout_Elements::Update(double Ctime, double Etime)
	{
        SampleBase::Update(Ctime, Etime);
	}

}