#include "texturingInst.hpp"
#include "MapHelper.hpp"
#include "TextureUtilities.h"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new TextureInst();
}
void TextureInst::CreateBuffers() 
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

    m_pDevice->CreateBuffer(Vdesc, &Vdata, &Vertex_Buffer);

    BufferDesc Idesc;
    Idesc.Name      = "Index Buffers";
    Idesc.BindFlags = BIND_INDEX_BUFFER;
    Idesc.Usage     = USAGE_IMMUTABLE;
    Idesc.Size      = sizeof(Indices);

    BufferData Idata;
    Idata.pData    = Indices;
    Idata.DataSize = sizeof(Indices);

    m_pDevice->CreateBuffer(Idesc, &Idata, &Index_Buffer);
}
void TextureInst::LoadTexture()
{
    TextureLoadInfo texInfo;
    texInfo.IsSRGB = true;

    RefCntAutoPtr<ITexture> tex;
    CreateTextureFromFile("NoExit.jpg", texInfo, m_pDevice, &tex);

    m_texture = tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
    m_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(m_texture);
}
void TextureInst::CreateUniformBuffer()
    {
    BufferDesc uniformDesc;
    uniformDesc.Name = "Uniform Constants Buffer";
    uniformDesc.BindFlags=BIND_UNIFORM_BUFFER;
    uniformDesc.Usage     = USAGE_DYNAMIC;
    uniformDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
    uniformDesc.Size           = sizeof(float4x4);
    m_pDevice->CreateBuffer(uniformDesc, nullptr, &m_Constants);
}
void TextureInst::CreatePipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
    PSOCreateInfo.PSODesc.Name         = "PIPLEINE TEXTURES";

    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

    ShaderCreateInfo ShaderCI;
    ShaderCI.Desc.UseCombinedTextureSamplers = true;
    ShaderCI.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_GLSL;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    ShaderCI.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> VS;
    {
        ShaderCI.Desc.Name       = "VS shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "VS.glsl";
        m_pDevice->CreateShader(ShaderCI, &VS);
    }
    RefCntAutoPtr<IShader> PS;
    {
        ShaderCI.Desc.Name       = "PS shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "PS.glsl";
        m_pDevice->CreateShader(ShaderCI, &PS);
    }

    PSOCreateInfo.pVS = VS;
    PSOCreateInfo.pPS = PS;


    LayoutElement elems[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 2, VT_FLOAT32, False}};
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);

    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    CreateUniformBuffer();

    ShaderResourceVariableDesc vars[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
    SamplerDesc sampler =
        {
            FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};
    ImmutableSamplerDesc immutables[]
    {
        {
            SHADER_TYPE_PIXEL, "g_Texture", sampler
        }
    };

    PSOCreateInfo.PSODesc.ResourceLayout.Variables = vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(vars);

    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers =immutables;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(immutables);

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo,&m_PSO);
    m_PSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_Constants);
    m_PSO->CreateShaderResourceBinding(&m_SRB, true);

}
void TextureInst::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipeline();
    CreateBuffers();
    LoadTexture();
}
void TextureInst::Update(double ctime, double etime)
{
    SampleBase::Update(ctime, etime);

    float4x4 ModelTranslation = float4x4::Translation(0.0f, 0.0f, 0.0f);
    float4x4 ModelRotation    = float4x4::RotationY(0.35f);

    float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);

    // Get pretransform matrix that rotates the scene according the surface orientation
    auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});
    auto Proj            = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

    MVP = ModelTranslation*ModelRotation * View * SrfPreTransform * Proj;
}
void TextureInst::Render()
{
    float ClearColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
    auto *pRTV=m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    
    {
        MapHelper<float4x4> VConstants(m_pImmediateContext, m_Constants, MAP_WRITE, MAP_FLAG_DISCARD);
        *VConstants= MVP.Transpose();
    }

    Uint64 offsets = 0;
    IBuffer*     pbuffer[] = {Vertex_Buffer};
    m_pImmediateContext->SetVertexBuffers(0,1,pbuffer,&offsets,RESOURCE_STATE_TRANSITION_MODE_TRANSITION,SET_VERTEX_BUFFERS_FLAG_RESET);
    m_pImmediateContext->SetIndexBuffer(Index_Buffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    m_pImmediateContext->SetPipelineState(m_PSO);
    m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    DrawIndexedAttribs dr;
    dr.IndexType = VT_UINT32;
    dr.NumIndices = 36;
    dr.Flags      = DRAW_FLAG_VERIFY_ALL;

    m_pImmediateContext->DrawIndexed(dr);
}

} // namespace Diligent