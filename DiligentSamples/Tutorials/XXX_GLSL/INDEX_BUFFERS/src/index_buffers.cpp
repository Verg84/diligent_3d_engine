#include "index_buffers.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new IndexBuffers();
}
void IndexBuffers::CreateVertexBuffers() 
{
    struct Vertex
    {
        float3 position;
    };
    Vertex P[] =
    {
            {float3(0.0,0.0,0.0)},
            {float3(1.0,0.0,0.0)},
            {float3(0.0,1.0,0.0)},
            {float3(1.0,1.0,0.0)}
    };

    BufferDesc VDesc;
    VDesc.Name = "VERTEX BUFFER";
    VDesc.Usage=USAGE_IMMUTABLE;
    VDesc.BindFlags = BIND_VERTEX_BUFFER;
    VDesc.Size      = sizeof(P);

    BufferData VData;
    VData.pData = P;
    VData.DataSize = sizeof(P);
    m_pDevice->CreateBuffer(VDesc, &VData, &VBuffers);
}
void IndexBuffers::CreateIndexBuffers()
{
    Uint32 Index[] =
        {
            0,3,2,
            0,1,3};
    BufferDesc IDesc;
    IDesc.Name = "INDEX BUFFERS";
    IDesc.BindFlags = BIND_INDEX_BUFFER;
    IDesc.Usage     = USAGE_IMMUTABLE;
    IDesc.Size      = sizeof(Index);

    BufferData IData;
    IData.pData=Index;
    IData.DataSize = sizeof(Index);
    m_pDevice->CreateBuffer(IDesc, &IData, &IBuffers);
    
}
void IndexBuffers::CreatePipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name = "PIPELINE INDEX BUFFERS";
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    PSOCreateInfo.GraphicsPipeline.NumRenderTargets=1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat       = m_pSwapChain->GetDesc().DepthBufferFormat;

    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;

    ShaderCreateInfo sci;
    sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_GLSL;
    sci.Desc.UseCombinedTextureSamplers = true;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    sci.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> VS;
    {
        sci.Desc.Name = "VERTEX SHADER";
        sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
        sci.EntryPoint      = "main";
        sci.FilePath        = "VS.glsl";
        m_pDevice->CreateShader(sci, &VS);
    }
    RefCntAutoPtr<IShader> PS;
    {
        sci.Desc.Name = "PIXEL SHADER";
        sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
        sci.EntryPoint = "main";
        sci.FilePath        = "PS.glsl";
        m_pDevice->CreateShader(sci, &PS);
    }

    PSOCreateInfo.pVS = VS;
    PSOCreateInfo.pPS = PS;

    LayoutElement vertex_attributes[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False}};
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = vertex_attributes;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(vertex_attributes);

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
}

void IndexBuffers::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipeline();
    CreateVertexBuffers();
    CreateIndexBuffers();
}

void IndexBuffers::Render()
{
    float ClearColor[]={0.35f,0.4f,0.3f,1.0f};
    auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV        = m_pSwapChain->GetDepthBufferDSV();

    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    
    const Uint64 offset = 0;
    IBuffer*     pBuffs[] = {VBuffers};
    
    m_pImmediateContext->SetVertexBuffers(
        0,
        1,
        pBuffs,
        &offset,
        RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
        SET_VERTEX_BUFFERS_FLAG_RESET);

    m_pImmediateContext->SetIndexBuffer(
        IBuffers,0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    m_pImmediateContext->SetPipelineState(m_pPSO);

    DrawIndexedAttribs drawAttrs;
    drawAttrs.IndexType = VT_UINT32;
    drawAttrs.NumIndices = 6;
    drawAttrs.Flags      = DRAW_FLAG_VERIFY_ALL;

    m_pImmediateContext->DrawIndexed(drawAttrs);
}

void IndexBuffers::Update(double c, double e)
{
    SampleBase::Update(c, e);
}
}