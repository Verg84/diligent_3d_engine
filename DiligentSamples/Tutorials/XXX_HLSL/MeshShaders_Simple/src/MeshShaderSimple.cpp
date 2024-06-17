#include"MeshShaderSimple.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new MeshShaderSimple();
}
void MeshShaderSimple::CreateMeshShaderPipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_MESH;
    PSOCreateInfo.PSODesc.Name         = "MESH SHADER PIPELINE";

    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
    
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable=false;
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology=PRIMITIVE_TOPOLOGY_UNDEFINED;

    ShaderCreateInfo sci;
    sci.SourceLanguage=SHADER_SOURCE_LANGUAGE_HLSL;
    sci.Desc.UseCombinedTextureSamplers = true;
    sci.ShaderCompiler                  = SHADER_COMPILER_DXC;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    sci.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> MS;
    {
        sci.Desc.Name = "MS";
        sci.Desc.ShaderType = SHADER_TYPE_MESH;
        sci.EntryPoint      = "main";
        sci.FilePath        = "simpleMS003.msh";
        m_pDevice->CreateShader(sci, &MS);
    }
    RefCntAutoPtr<IShader> PS;
    {
        sci.Desc.Name = "PS";
        sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
        sci.EntryPoint      = "main";
        sci.FilePath        = "simplePS.psh";
        m_pDevice->CreateShader(sci, &PS);
    }

    PSOCreateInfo.pMS = MS;
    PSOCreateInfo.pPS = PS;

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
}
void MeshShaderSimple::Initialize(const SampleInitInfo &InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreateMeshShaderPipeline();
}
void MeshShaderSimple::Update(double ctime, double etime)
{
    SampleBase::Update(ctime, etime);
}
void MeshShaderSimple::Render()
{
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
    // Clear the back buffer
    const float ClearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);


    m_pImmediateContext->SetPipelineState(m_pPSO);

    DrawMeshAttribs drawAttrs;
    drawAttrs.ThreadGroupCount = 1;
    drawAttrs.Flags            = DRAW_FLAG_VERIFY_ALL;
    drawAttrs.Flags            = DRAW_FLAG_VERIFY_ALL;
    m_pImmediateContext->DrawMesh(drawAttrs);
}

}