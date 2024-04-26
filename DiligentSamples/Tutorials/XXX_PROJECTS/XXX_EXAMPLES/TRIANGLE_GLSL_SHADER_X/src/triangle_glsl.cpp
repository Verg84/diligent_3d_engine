#include"triangle_glsl.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Triangle_glsl();
	}
	void Triangle_glsl::Initialize(const SampleInitInfo &InitInfo)
	{
        SampleBase::Initialize(InitInfo);

		GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "Pipeline GLSL loading";
		PSOCreateInfo.PSODesc.PipelineType=PIPELINE_TYPE_GRAPHICS;

		PSOCreateInfo.GraphicsPipeline.NumRenderTargets=1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat       = m_pSwapChain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;

		// SHADERS
        ShaderCreateInfo sci;
        sci.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_GLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        RefCntAutoPtr<IShader> vertex_shader;
        {
            sci.Desc.Name       = "Vertex Shader";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = "VS.vert";
            m_pDevice->CreateShader(sci, &vertex_shader);
        }
        RefCntAutoPtr<IShader> frag_shader;
        {
            sci.Desc.Name       = "Pixel Shader";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "PS.frag";
            m_pDevice->CreateShader(sci, &frag_shader);
        }
        PSOCreateInfo.pVS = vertex_shader;
        PSOCreateInfo.pPS = frag_shader;


        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

	}
    void Triangle_glsl::Update(double currentTime, double elapsedTime)
    {	
		SampleBase::Update(currentTime, elapsedTime);
	}

    void Triangle_glsl::Render()
    {
        float ClearColor[] = {0.35f, 0.35f, 0.35f, 1.0f};
        auto*  pRTV          = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV          = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG_NONE,1.0,0,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        m_pImmediateContext->SetPipelineState(m_pPSO);

        DrawAttribs draw;
        draw.NumVertices=3;
        m_pImmediateContext->Draw(draw);


    }

    }