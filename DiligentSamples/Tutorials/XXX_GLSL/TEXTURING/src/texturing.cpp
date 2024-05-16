#include "texturing.hpp"
#include "MapHelper.hpp"
#include "TextureUtilities.h"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new Texturing();
}

void Texturing::LoadCubeBuffers()
{
    struct Vertex
    {
        float3 position;
        float2 uv;
    };

     constexpr Vertex CubePositions[] =
        {
            {float3{-1, -1, -1}, float2{0, 1}},
            {float3{-1, +1, -1}, float2{0, 0}},
            {float3{+1, +1, -1}, float2{1, 0}},
            {float3{+1, -1, -1}, float2{1, 1}},

            {float3{-1, -1, -1}, float2{0, 1}},
            {float3{-1, -1, +1}, float2{0, 0}},
            {float3{+1, -1, +1}, float2{1, 0}},
            {float3{+1, -1, -1}, float2{1, 1}},

            {float3{+1, -1, -1}, float2{0, 1}},
            {float3{+1, -1, +1}, float2{1, 1}},
            {float3{+1, +1, +1}, float2{1, 0}},
            {float3{+1, +1, -1}, float2{0, 0}},

            {float3{+1, +1, -1}, float2{0, 1}},
            {float3{+1, +1, +1}, float2{0, 0}},
            {float3{-1, +1, +1}, float2{1, 0}},
            {float3{-1, +1, -1}, float2{1, 1}},

            {float3{-1, +1, -1}, float2{1, 0}},
            {float3{-1, +1, +1}, float2{0, 0}},
            {float3{-1, -1, +1}, float2{0, 1}},
            {float3{-1, -1, -1}, float2{1, 1}},

            {float3{-1, -1, +1}, float2{1, 1}},
            {float3{+1, -1, +1}, float2{0, 1}},
            {float3{+1, +1, +1}, float2{0, 0}},
            {float3{-1, +1, +1}, float2{1, 0}},
        };


     constexpr Uint32 Indices[] =
         {
             2, 0, 1, 2, 3, 0,
             4, 6, 5, 4, 7, 6,
             8, 10, 9, 8, 11, 10,
             12, 14, 13, 12, 15, 14,
             16, 18, 17, 16, 19, 18,
             20, 21, 22, 20, 22, 23};

    BufferDesc Vdesc;
    Vdesc.Name      = "Vertex Buffers";
    Vdesc.Usage     = USAGE_IMMUTABLE;
    Vdesc.BindFlags = BIND_VERTEX_BUFFER;
    Vdesc.Size      = sizeof(CubePositions);

    BufferData Vdata;
    Vdata.pData    = CubePositions;
    Vdata.DataSize = sizeof(CubePositions);

    m_pDevice->CreateBuffer(Vdesc, &Vdata, &VSBuffer);

    BufferDesc Idesc;
    Idesc.Name      = "Index Buffers";
    Idesc.BindFlags = BIND_INDEX_BUFFER;
    Idesc.Usage     = USAGE_IMMUTABLE;
    Idesc.Size      = sizeof(Indices);

    BufferData Idata;
    Idata.pData    = Indices;
    Idata.DataSize = sizeof(Indices);

    m_pDevice->CreateBuffer(Idesc, &Idata, &IndexBuffer);
}

void Texturing::LoadConstantsUniformBuffer()
{
    BufferDesc bufDesc;
    bufDesc.Name           = "Uniform Buffer";
    bufDesc.Usage          = USAGE_DYNAMIC;
    bufDesc.BindFlags      = BIND_UNIFORM_BUFFER;
    bufDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
    bufDesc.Size           = sizeof(float4x4);

    m_pDevice->CreateBuffer(bufDesc, nullptr,&VSConstants);
}
void Texturing::CreatePipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name         = "PIPELINE CUBE";
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

    ShaderCreateInfo sci;
    sci.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_GLSL;
    sci.Desc.UseCombinedTextureSamplers = true;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    sci.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> VS;
    {
        sci.Desc.Name       = "Cube Vertex Shader";
        sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
        sci.EntryPoint      = "main";
        sci.FilePath        = "VS.glsl";
        m_pDevice->CreateShader(sci, &VS);
    }
    RefCntAutoPtr<IShader> PS;
    {
        sci.Desc.Name       = "PIXEL SHADER CUBE";
        sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
        sci.EntryPoint      = "main";
        sci.FilePath        = "PS.glsl";
        m_pDevice->CreateShader(sci, &PS);
    }

    PSOCreateInfo.pVS = VS;
    PSOCreateInfo.pPS = PS;

    LayoutElement elems[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 2, VT_FLOAT32, False}};
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;

    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    LoadConstantsUniformBuffer();

    ShaderResourceVariableDesc Vars[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
    PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables =_countof(Vars);

    SamplerDesc sampler =
        {
            FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};
    ImmutableSamplerDesc imtbl[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", sampler}};
    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = imtbl;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(imtbl);

    a
    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
    m_pPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(VSConstants);

    m_pPSO->CreateShaderResourceBinding(&m_SRB, true);
}

void Texturing::LoadTexture() 
{
    TextureLoadInfo texLoadInfo;
    texLoadInfo.IsSRGB = true;

    RefCntAutoPtr<ITexture> tex;
    CreateTextureFromFile("DGLogo.png", texLoadInfo, m_pDevice, &tex);

    m_TextureSRV = tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

    m_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(m_TextureSRV);
}
void Texturing::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipeline();
    LoadCubeBuffers();
    LoadTexture();
}
void Texturing::Render()
{
    

    float ClearColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
    auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        MapHelper<float4x4> Constants(
            m_pImmediateContext,
            VSConstants,
            MAP_WRITE,
            MAP_FLAG_DISCARD);
        *Constants = MVP.Transpose();
    }

   
    IBuffer* buf[]   = {VSBuffer};
    Uint64   offsets  = 0;
    m_pImmediateContext->SetVertexBuffers(
        0,
        1,
        buf,
        &offsets,
        RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
        SET_VERTEX_BUFFERS_FLAG_RESET);

    m_pImmediateContext->SetIndexBuffer(
        IndexBuffer,
        0,
        RESOURCE_STATE_TRANSITION_MODE_TRANSITION
    );

    m_pImmediateContext->SetPipelineState(m_pPSO);
    m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    DrawIndexedAttribs atr;
    atr.IndexType  = VT_UINT32;
    atr.NumIndices = 36;
    atr.Flags      = DRAW_FLAG_VERIFY_ALL;

    m_pImmediateContext->DrawIndexed(atr);
}
void Texturing::Update(double c, double e)
{
    SampleBase::Update(c, e);

    // Apply rotation
    float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(c) * 1.0f);

    // Camera is at (0, 0, -5) looking along the Z axis
    float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);

    // Get pretransform matrix that rotates the scene according the surface orientation
    auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

    // Get projection matrix adjusted to the current screen orientation
    auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

    // Compute world-view-projection matrix
    MVP = CubeModelTransform * View * SrfPreTransform * Proj;
}
} // namespace Diligent