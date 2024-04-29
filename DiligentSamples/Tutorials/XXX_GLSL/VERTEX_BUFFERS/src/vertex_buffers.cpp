#include "vertex_buffers.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new VertexBuffers();
}
void VertexBuffers::CreateVertexBuffer() 
{
    struct Vertex
    {
        float3 position;
    };

    Vertex Positions[] =
        {
            {float3(0.0, 0.0, 0.0)},
            {float3(1.0, 0.0, 0.0)},
            {float3(0.0, 1.0, 0.0)}};

    BufferDesc vertexDesc;
    vertexDesc.Name = "Vertex Buffer";
    vertexDesc.BindFlags = BIND_VERTEX_BUFFER;
    vertexDesc.Usage     = USAGE_IMMUTABLE;
    vertexDesc.Size      = sizeof(Positions);

    BufferData vertexData;
    vertexData.pData = Positions;
    vertexData.DataSize = sizeof(Positions);

    m_pDevice->CreateBuffer(vertexDesc, &vertexData, &VBuffer);
}
void VertexBuffers::CreatePipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name = "Pipeline Vertex Buffers";
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    PSOCreateInfo.GraphicsPipeline.NumRenderTargets=1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat       = m_pSwapChain->GetDesc().DepthBufferFormat;

    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;

    ShaderCreateInfo sci;
    sci.Desc.UseCombinedTextureSamplers = true;
    sci.SourceLanguage=SHADER_SOURCE_LANGUAGE_GLSL;

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

  

    LayoutElement vertexAttributes[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False}};

    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(vertexAttributes);
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = vertexAttributes;

    PSOCreateInfo.pVS = VS;
    PSOCreateInfo.pPS = PS;

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo,&m_pPSO);
}
void VertexBuffers::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipeline();
    CreateVertexBuffer();
}
void VertexBuffers::Update(double c, double e)
{
    SampleBase::Update(c, e);
}

void VertexBuffers::Render()
{
    float ClearColor[] = {0.35f, 0.35f, 0.35f, 1.0};
    auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

    m_pImmediateContext->ClearRenderTarget(pRTV,ClearColor,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG_NONE, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    
    const Uint64 offset=0;
    IBuffer*     pbuffer[] = {VBuffer};
    m_pImmediateContext->SetVertexBuffers(
                        0,
                        1,
                        pbuffer,
                        &offset,
                        RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
                        SET_VERTEX_BUFFERS_FLAG_RESET);

    m_pImmediateContext->SetPipelineState(m_pPSO);

    DrawAttribs draw;
    draw.NumVertices = 3;

    m_pImmediateContext->Draw(draw);
}
}