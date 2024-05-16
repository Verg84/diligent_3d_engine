#include"pyramid.hpp"
#include"MapHelper.hpp"
#include"TextureUtilities.h"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new Pyramid2d();
}

void Pyramid2d::LoadBuffers()
{
    struct Vertices
    {
        float3 pos;
        float2 uv;
    };

    Vertices Positions[] =
        {
            {float3(-0.8f, -0.8f, 0.0f), float2(0.0f, 0.0f)},
            {float3(0.8f, -0.8f, 0.0f), float2(1.0f, 0.0f)},
            {float3(0.0f, 0.0f, 0.0f), float2(0.5f, 0.5f)}};

    BufferDesc vdesc;
    vdesc.Name      = "Vertex Buffer";
    vdesc.BindFlags = BIND_VERTEX_BUFFER;
    vdesc.Usage     = USAGE_IMMUTABLE;
    vdesc.Size      = sizeof(Positions);
    BufferData vdata;
    vdata.pData    = Positions;
    vdata.DataSize = sizeof(Positions);
    m_pDevice->CreateBuffer(vdesc, &vdata, &Vertex_Buffer);
}
void Pyramid2d::LoadTexture()
{
    TextureLoadInfo texLoadInfo;
    texLoadInfo.IsSRGB = true;

    RefCntAutoPtr<ITexture> tex;
    CreateTextureFromFile("pyramid_texture.png", texLoadInfo, m_pDevice, &tex);
    m_Texture = tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

    m_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "sampl")->Set(m_Texture);
}
void Pyramid2d::CreateUniformfBuffer()
{
    BufferDesc fDesc;
    fDesc.Name           = "Constants buffer";
    fDesc.Usage          = USAGE_DYNAMIC;
    fDesc.BindFlags      = BIND_UNIFORM_BUFFER;
    fDesc.Size           = sizeof(float3);
    fDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
    m_pDevice->CreateBuffer(fDesc, nullptr, &m_ConstantsBuffer);
}

void Pyramid2d::CreatePipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name         = "Pipeline";
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_NONE;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;

    ShaderCreateInfo ShaderCI;
    ShaderCI.Desc.UseCombinedTextureSamplers = true;
    ShaderCI.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_GLSL;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    ShaderCI.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> VS;
    {
        ShaderCI.Desc.Name       = "VS";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "VS.glsl";
        m_pDevice->CreateShader(ShaderCI, &VS);
    }

    RefCntAutoPtr<IShader> PS;
    {
        ShaderCI.Desc.Name       = "PS";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "PS.glsl";
        m_pDevice->CreateShader(ShaderCI, &PS);
    }

    PSOCreateInfo.pVS = VS;
    PSOCreateInfo.pPS = PS;

    CreateUniformfBuffer();

    LayoutElement elems[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 2, VT_FLOAT32, False}};
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);

    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    ShaderResourceVariableDesc vars[] =
        {
            {SHADER_TYPE_PIXEL, "sampl", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
    SamplerDesc samplerD =
        {
            FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};
    ImmutableSamplerDesc immut[] =
        {
            {SHADER_TYPE_PIXEL, "sampl", samplerD}};
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(immut);
    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers    = immut;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables         = _countof(vars);
    PSOCreateInfo.PSODesc.ResourceLayout.Variables            = vars;

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_PSO);

    m_PSO->CreateShaderResourceBinding(&m_SRB, true);
    m_PSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constf")->Set(m_ConstantsBuffer);
}

void Pyramid2d::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);

    CreatePipeline();
    LoadBuffers();
    LoadTexture();
}
void Pyramid2d::Render()
{
    float ClearColor[] = {0.0, 0.0, 1.0, 1.0};
    auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        MapHelper<float3> fConstants(m_pImmediateContext, m_ConstantsBuffer, MAP_WRITE, MAP_FLAG_DISCARD);
        *fConstants = offset;
    }

    const Uint64 offsetp    = 0;
    IBuffer*     pbuffers[] = {Vertex_Buffer};
    m_pImmediateContext->SetVertexBuffers(0, 1, pbuffers, &offsetp, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);

    m_pImmediateContext->SetPipelineState(m_PSO);
    m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    DrawAttribs dr;
    dr.NumVertices = 3;
    dr.Flags       = DRAW_FLAG_VERIFY_ALL;

    m_pImmediateContext->Draw(dr);
}
void Pyramid2d::Update(double c, double e)
{
    SampleBase::Update(c, e);

    offset += float3((float)c, 0.0f, 0.0f);
    
}
} // namespace Diligent