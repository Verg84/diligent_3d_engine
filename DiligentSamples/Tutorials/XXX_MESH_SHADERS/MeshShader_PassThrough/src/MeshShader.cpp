#include"MeshShader.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new MeshShader();
}
void MeshShader::CreatePipelineState()
{
    GraphicsPipelineStateCreateInfo PSO;
    PSO.PSODesc.Name = "PIPELINE - MS PASS THROUGH";
    PSO.PSODesc.PipelineType = PIPELINE_TYPE_MESH;
    PSO.GraphicsPipeline.NumRenderTargets = 1;
    PSO.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSO.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
    PSO.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
    PSO.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
    PSO.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
    PSO.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
    PSO.GraphicsPipeline.PrimitiveTopology=PRIMITIVE_TOPOLOGY_UNDEFINED;

    ShaderCreateInfo sci;
    sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
    sci.Desc.UseCombinedTextureSamplers = true;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    sci.pShaderSourceStreamFactory = sf;
    sci.ShaderCompiler=SHADER_COMPILER_DXC;

    RefCntAutoPtr<IShader> MS;
    {
        sci.Desc.Name = "MS";
        sci.Desc.ShaderType = SHADER_TYPE_MESH;
        sci.EntryPoint      = "main";
        sci.FilePath        = "pass_through003_MS.msh";
        m_pDevice->CreateShader(sci, &MS);
    }
    RefCntAutoPtr<IShader> PS;
    {
        sci.Desc.Name = "PS";
        sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
        sci.EntryPoint      = "main";
        sci.FilePath        = "PS.psh";
        m_pDevice->CreateShader(sci, &PS);
    }
    PSO.pMS = MS;
    PSO.pPS = PS;

    m_pDevice->CreateGraphicsPipelineState(PSO, &m_pPSO);
    VERIFY_EXPR(m_pPSO !=nullptr);
}
void MeshShader::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipelineState();
}
void MeshShader::Update(double ctime, double etime)
{
    SampleBase::Update(ctime, etime);
}
void MeshShader::Render()
{
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
    float ClearColor[] = {0.1f, 0.1f, 0.1f, 1.0};

    DrawMeshAttribs drm;
    drm.ThreadGroupCount = 1;
    drm.Flags            = DRAW_FLAG_VERIFY_ALL;

    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_NONE);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG_NONE, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->SetPipelineState(m_pPSO);
    m_pImmediateContext->DrawMesh(drm);
}
}