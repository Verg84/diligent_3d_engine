#include"GeomShader002.hpp"
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
            float3 position;
        };
        Vertex P[] =
            {
                {float3(0.0, 0.0, 0.0)},
                {float3(1.0, 0.0, 0.0)},
                {float3(0.0, 1.0, 0.0)},
                {float3(1.0, 1.0, 0.0)}};

        BufferDesc VDesc;
        VDesc.Name      = "VERTEX BUFFER";
        VDesc.Usage     = USAGE_IMMUTABLE;
        VDesc.BindFlags = BIND_VERTEX_BUFFER;
        VDesc.Size      = sizeof(P);

        BufferData VData;
        VData.pData    = P;
        VData.DataSize = sizeof(P);
        m_pDevice->CreateBuffer(VDesc, &VData, &VertexBuffer);
   }
	void GeomShader002::CreatePipelineState()
	{
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "PIPELINE GEOMETRY SHADER";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode=CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;

		ShaderCreateInfo sci;
        sci.Desc.UseCombinedTextureSamplers = true;
        sci.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_GLSL;

		RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        RefCntAutoPtr<IShader> VS;
        {
            sci.Desc.Name = "VS";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = "VS.glsl";
            m_pDevice->CreateShader(sci, &VS);
        }
        RefCntAutoPtr<IShader> GS;
        {
            sci.Desc.Name = "GS";
            sci.Desc.ShaderType = SHADER_TYPE_GEOMETRY;
            sci.EntryPoint      = "main";
            sci.FilePath        = "GS.glsl";
            m_pDevice->CreateShader(sci, &GS);
        }
        RefCntAutoPtr<IShader> PS;
        {
            sci.Desc.Name = "PS";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "PS.glsl";
            m_pDevice->CreateShader(sci, &PS);
        }

        PSOCreateInfo.pVS = VS;
        PSOCreateInfo.pGS = GS;
        PSOCreateInfo.pPS = PS;

        LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False}};
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
	void GeomShader002::Render()
	{
        float ClearColor[] = {0.1f, 0.1f, 0.2f, 1.0f};
        auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV,CLEAR_DEPTH_FLAG,1.0,0,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        const Uint64 offset = 0;
        IBuffer*     pbuffer[] = {VertexBuffer};
        m_pImmediateContext->SetVertexBuffers(0,1,pbuffer,&offset,RESOURCE_STATE_TRANSITION_MODE_TRANSITION,SET_VERTEX_BUFFERS_FLAG_RESET);
        
        m_pImmediateContext->SetPipelineState(m_PSO);

        DrawAttribs dr;
        dr.NumVertices = 4;

        m_pImmediateContext->Draw(dr);
	}
	void GeomShader002::Update(double ctime,double etime)
	{

	}

    }