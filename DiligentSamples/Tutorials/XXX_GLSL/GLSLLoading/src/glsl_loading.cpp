#include"glsl_loading.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new GLSLLoad();
}

void GLSLLoad::CreatePipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name = "PIPELINE glsl shaders loading";
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    PSOCreateInfo.GraphicsPipeline.NumRenderTargets=1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat       = m_pSwapChain->GetDesc().DepthBufferFormat;

    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

    ShaderCreateInfo sci;
    sci.Desc.UseCombinedTextureSamplers=true;
    sci.SourceLanguage=SHADER_SOURCE_LANGUAGE_GLSL;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    sci.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> VS;
    {
        sci.Desc.Name = "Vertex Shader";
        sci.Desc.ShaderType=SHADER_TYPE_VERTEX;
        sci.EntryPoint      = "main";
        sci.FilePath       = "VS.glsl";
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
    PSOCreateInfo.pVS=VS;
    PSOCreateInfo.pPS = PS;

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

}
void GLSLLoad::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipeline();
}
void GLSLLoad::Update(double c, double e)
{
    SampleBase::Update(c, e);
}
void GLSLLoad::Render()
{
    float ClearColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
    auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

    m_pImmediateContext->ClearRenderTarget(pRTV,ClearColor,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG_NONE, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    
    m_pImmediateContext->SetPipelineState(m_pPSO);

    DrawAttribs draw;
    draw.NumVertices=3;
    m_pImmediateContext->Draw(draw);


}
}