#include"triangle.hpp"

namespace Diligent
{
    SampleBase* CreateSample()
    {
        return new Triangle();
    }

    void Triangle::Initialize(const SampleInitInfo &InitInfo)
    {
        SampleBase::Initialize(InitInfo);

        GraphicsPipelineStateCreateInfo PSOCreateInfo;

        PSOCreateInfo.PSODesc.Name = "Simple Triangle Graphics Pipeline";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        // Set ouputs for the Pipeline to use
        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]=m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;

        ShaderCreateInfo ShaderCI;
        ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        ShaderCI.Desc.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        ShaderCI.pShaderSourceStreamFactory = sf;

        RefCntAutoPtr<IShader> vertex_shader;
        {
            ShaderCI.Desc.Name = "VS triangle shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
            ShaderCI.EntryPoint = "main";
            ShaderCI.FilePath        = "VS.vsh";
            m_pDevice->CreateShader(ShaderCI, &vertex_shader);
        }
        RefCntAutoPtr<IShader> pixel_shader;
        {
            ShaderCI.Desc.Name = "PS triangle shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "PS.psh";
            m_pDevice->CreateShader(ShaderCI, &pixel_shader);
        }
        PSOCreateInfo.pVS = vertex_shader;
        PSOCreateInfo.pPS = pixel_shader;

        m_pDevice->CreatePipelineState(PSOCreateInfo, &m_pPSO);
    }
    void Triangle::Render()
    {
        const float ClearColor[] = {0.35f, 0.35f, 0.35f, 1.0};

        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        m_pImmediateContext->SetPipelineState(m_pPSO);

        DrawAttribs draw;
        draw.NumVertices = 3;
        m_pImmediateContext->Draw(draw);
    }
    void Triangle::Update(double currentTime,double elapsedTime)
    {
        SampleBase::Update(currentTime, elapsedTime);
    }
 }