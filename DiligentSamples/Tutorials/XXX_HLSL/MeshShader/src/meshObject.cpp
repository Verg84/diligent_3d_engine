#include"meshObject.hpp"
#include"Object.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new MObject();
	}
	void MObject::CreatePipeline()
	{
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "OBJ PIPELINE";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
		
		PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

		PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
		PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode=CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology=PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		ShaderCreateInfo ShaderCI;
        ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        ShaderCI.Desc.UseCombinedTextureSamplers = true;

		RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr,&sf);
        ShaderCI.pShaderSourceStreamFactory = sf;

		RefCntAutoPtr<IShader> VS;
		{
            ShaderCI.Desc.Name = "VERTEX SHADER";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "VS.vsh";
            m_pDevice->CreateShader(ShaderCI, &VS);
		}

		RefCntAutoPtr<IShader> PS;
		{
            ShaderCI.Desc.Name = "PIXEL SHADER";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "PS.psh";
            m_pDevice->CreateShader(ShaderCI, &PS);
		}
		
		PSOCreateInfo.pVS = VS;
        PSOCreateInfo.pPS = PS;

		LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False}};
        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);

		m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo,&m_pPSO);
	}
	void MObject::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        CreatePipeline();
        VERIFY_EXPR(m_pPSO != nullptr);

        
		
	
        
        m_VertexBuffer               = Object::CreateVertexBuffer(m_pDevice, Object::VERTEX_COMPONENT_FLAG_POSITION);
        VERIFY_EXPR(m_VertexBuffer != nullptr);
		m_IndexBuffer                = Object::CreateIndexBuffer(m_pDevice);
        VERIFY_EXPR(m_IndexBuffer != nullptr);
			

	}
	void MObject::Render()
	{
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        // Clear the back buffer
        const float ClearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

      

        // Bind vertex and index buffers
        const Uint64 offset   = 0;
        IBuffer*     pBuffs[] = {m_VertexBuffer};
        m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(m_IndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // Set the pipeline state
        m_pImmediateContext->SetPipelineState(m_pPSO);
        // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
        // makes sure that resources are transitioned to required states.

        DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
        DrawAttrs.IndexType  = VT_UINT32; // Index type
        DrawAttrs.NumIndices = 3;
        // Verify the state of vertex and index buffers
        DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(DrawAttrs);
	}
	void MObject::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
	}
    }