#include"Textures.hpp"
#include "MapHelper.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new Textures();
}

void Textures::CreateUniformBuffer()
{
    BufferDesc udesc;
    udesc.Name = "Vertex Uniforms";
    udesc.BindFlags = BIND_UNIFORM_BUFFER;
    udesc.Usage     = USAGE_DYNAMIC;
    udesc.CPUAccessFlags = CPU_ACCESS_WRITE;
    udesc.Size           = sizeof(float4x4);
    m_pDevice->CreateBuffer(udesc, nullptr, &Uniform_Buffer);
               
}

void Textures::CreateBuffers()
{
    struct Vertex
    {
        float3 Pos;
        float2 uv;
    };
    Vertex CubeVerts[] =
        {
            {float3(-1, -1, -1), float2(0, 1)},
            {float3(-1, +1, -1), float2(0, 0)},
            {float3(+1, +1, -1), float2(1, 0)},
            {float3(+1, -1, -1), float2(1, 1)},

            {float3(-1, -1, -1), float2(0, 1)},
            {float3(-1, -1, +1), float2(0, 0)},
            {float3(+1, -1, +1), float2(1, 0)},
            {float3(+1, -1, -1), float2(1, 1)},

            {float3(+1, -1, -1), float2(0, 1)},
            {float3(+1, -1, +1), float2(1, 1)},
            {float3(+1, +1, +1), float2(1, 0)},
            {float3(+1, +1, -1), float2(0, 0)},

            {float3(+1, +1, -1), float2(0, 1)},
            {float3(+1, +1, +1), float2(0, 0)},
            {float3(-1, +1, +1), float2(1, 0)},
            {float3(-1, +1, -1), float2(1, 1)},

            {float3(-1, +1, -1), float2(1, 0)},
            {float3(-1, +1, +1), float2(0, 0)},
            {float3(-1, -1, +1), float2(0, 1)},
            {float3(-1, -1, -1), float2(1, 1)},

            {float3(-1, -1, +1), float2(1, 1)},
            {float3(+1, -1, +1), float2(0, 1)},
            {float3(+1, +1, +1), float2(0, 0)},
            {float3(-1, +1, +1), float2(1, 0)}};

            Uint32 Indices[] =
            {
                2, 0, 1, 2, 3, 0,
                4, 6, 5, 4, 7, 6,
                8, 10, 9, 8, 11, 10,
                12, 14, 13, 12, 15, 14,
                16, 18, 17, 16, 19, 18,
                20, 21, 22, 20, 22, 23};
    
            BufferDesc vdesc;
            vdesc.Name = "Vertex Buffer";
            vdesc.BindFlags = BIND_VERTEX_BUFFER;
            vdesc.Usage     = USAGE_IMMUTABLE;
            vdesc.Size      = sizeof(CubeVerts);
            BufferData vdata;
            vdata.pData = CubeVerts;
            vdata.DataSize = sizeof(CubeVerts);
            m_pDevice->CreateBuffer(vdesc, &vdata, &Vertex_Buffer);

            BufferDesc idesc;
            idesc.Name = "Index Buffer";
            idesc.BindFlags = BIND_INDEX_BUFFER;
            idesc.Usage     = USAGE_IMMUTABLE;
            idesc.Size      = sizeof(Indices);
            BufferData idata;
            idata.pData=Indices;
            idata.DataSize = sizeof(Indices);
            m_pDevice->CreateBuffer(idesc, &idata, &Index_Buffer);


}

void Textures::CreateTexture()
{
    TextureLoadInfo LoadInfo;
    LoadInfo.IsSRGB = True;
    RefCntAutoPtr<ITexture> tex;
    CreateTextureFromFile("DGLogo.png", LoadInfo,m_pDevice,&tex);
    tex_resource = tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
    resource_binding->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(tex_resource);
}

void Textures::CreatePipelineState()
{
    GraphicsPipelineStateCreateInfo pso;
    pso.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
    pso.PSODesc.Name         = "Textures Pipeline";
    
    pso.GraphicsPipeline.NumRenderTargets = 1;
    pso.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    pso.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
    pso.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pso.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
    pso.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;
    
    ShaderCreateInfo sci;
    
    sci.Desc.UseCombinedTextureSamplers = true;
    sci.SourceLanguage=SHADER_SOURCE_LANGUAGE_HLSL;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    sci.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> vs;
    {
        sci.Desc.Name = "Vertex Shader";
        sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
        sci.EntryPoint      = "main";
        sci.FilePath        = "texture_vertex_shader_source.vsh";
        m_pDevice->CreateShader(sci, &vs);
    }
    RefCntAutoPtr<IShader> ps;
    {
        sci.Desc.Name = "Pixel Shader";
        sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
        sci.EntryPoint      = "main";
        sci.FilePath        = "texture_pixel_shader_source.psh";
        m_pDevice->CreateShader(sci, &ps);
    }
    pso.pVS = vs;
    pso.pPS = ps;

    pso.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    CreateUniformBuffer();

    LayoutElement elems[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 2, VT_FLOAT32, False}};
    pso.GraphicsPipeline.InputLayout.LayoutElements = elems;
    pso.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);

    ShaderResourceVariableDesc Vars[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
    pso.PSODesc.ResourceLayout.Variables = Vars;
    pso.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    SamplerDesc sampler{
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};
    ImmutableSamplerDesc imm[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", sampler}};
    pso.PSODesc.ResourceLayout.ImmutableSamplers = imm;
    pso.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(imm);

    m_pDevice->CreateGraphicsPipelineState(pso, &pipeline_handler);
    pipeline_handler->GetStaticVariableByName(SHADER_TYPE_VERTEX, "VertexShaderUniform")->Set(Uniform_Buffer);
    pipeline_handler->CreateShaderResourceBinding(&resource_binding, true);


}

void Textures::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipelineState();
    CreateBuffers();
    CreateTexture();
}
void Textures::Render()
{
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
    float ClearColor[] = {0.21f, 0.87f, 0.99f, 1.0f};
    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV,CLEAR_DEPTH_FLAG,1.0f,0,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        MapHelper<float4x4> Constants(m_pImmediateContext, Uniform_Buffer, MAP_WRITE, MAP_FLAG_DISCARD);
        *Constants= ModelViewProj.Transpose();
    }

    const Uint64 offset = 0;
    IBuffer*     pBuffs[] = {Vertex_Buffer};
    m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    m_pImmediateContext->SetIndexBuffer(Index_Buffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    m_pImmediateContext->SetPipelineState(pipeline_handler);
    m_pImmediateContext->CommitShaderResources(resource_binding,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    DrawIndexedAttribs draw;
    draw.NumIndices = 36;
    draw.IndexType  = VT_UINT32;
    draw.Flags      = DRAW_FLAG_VERIFY_ALL;
    m_pImmediateContext->DrawIndexed(draw);

}

void Textures::Update(double ctime, double etime)
{
    SampleBase::Update(ctime, etime);
    // Apply rotation
    float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(ctime) * 1.0f) * float4x4::RotationX(-PI_F * 0.1f);

    // Camera is at (0, 0, -5) looking along the Z axis
    float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);

    // Get pretransform matrix that rotates the scene according the surface orientation
    auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

    // Get projection matrix adjusted to the current screen orientation
    auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

    // Compute world-view-projection matrix
    ModelViewProj = CubeModelTransform * View * SrfPreTransform * Proj;
}

} // namespace Diligent