#include"XXX_TexturedCube.hpp"
#include "BasicMath.hpp"
#include "TextureUtilities.h"
#include "GraphicsTypesX.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new TexCube();
}
RefCntAutoPtr<IBuffer> TexCube::CreateVertexBuffer(IRenderDevice* Device, VERTEX_COMPONENTS_FLAGS VertexFlags, BIND_FLAGS BindFlags, BUFFER_MODE Mode)
{
    VERIFY_EXPR(VertexFlags != FLAG_NONE);
    const Uint32 TotalVertexComponents =
        ((VertexFlags & FLAG_POS) ? 3 : 0) +
        ((VertexFlags & FLAG_NORMAL) ? 3 : 0) +
        ((VertexFlags & FLAG_TEXTURE) ? 2 : 0);

    std::vector<float> VertexData(size_t{TotalVertexComponents} * NV);
    auto               it = VertexData.begin();
    for (Uint32 v = 0; v < NV; ++v)
    {
        if (VertexFlags & FLAG_POS)
        {
            const auto& Pos{Positions[v]};
            *(it++) = Pos.x;
            *(it++) = Pos.y;
            *(it++) = Pos.z;
        }
        if (VertexFlags & FLAG_NORMAL)
        {
            const auto& N{Normals[v]};
            *(it++) = N.x;
            *(it++) = N.y;
            *(it++) = N.z;
        }
        if (VertexFlags & FLAG_TEXTURE)
        {
            const auto& UV{TexCoords[v]};
            *(it++) = UV.x;
            *(it++) = UV.y;
        }
    }
    VERIFY_EXPR(it == VertexData.end());

    BufferDesc vbdesc;
    vbdesc.Name      = "Vertex Buffer";
    vbdesc.Usage     = USAGE_IMMUTABLE;
    vbdesc.BindFlags = BindFlags;
    vbdesc.Size      = static_cast<Uint64>(VertexData.size() * sizeof(VertexData[0]));
    vbdesc.Mode      = Mode;
    if (Mode != BUFFER_MODE_UNDEFINED)
    {
        vbdesc.ElementByteStride = TotalVertexComponents * sizeof(VertexData[0]);
    }
    BufferData vbd;
    vbd.pData    = VertexData.data();
    vbd.DataSize = vbdesc.Size;
    RefCntAutoPtr<IBuffer> v_buffer_handler;
    Device->CreateBuffer(vbdesc, &vbd, &v_buffer_handler);
    return vertex_buffer_handler;
}
RefCntAutoPtr<IBuffer> TexCube::CreateIndexBuffer(IRenderDevice* Device, BIND_FLAGS BindFlags, BUFFER_MODE Mode)
{
    BufferDesc idesc;
    idesc.Name      = "Indices";
    idesc.Usage     = USAGE_IMMUTABLE;
    idesc.BindFlags = BindFlags;
    idesc.Size      = sizeof(Indices);
    idesc.Mode      = Mode;
    if (Mode != BUFFER_MODE_UNDEFINED)
        idesc.ElementByteStride = sizeof(Indices[0]);
    BufferData idata;
    idata.pData    = Indices.data();
    idata.DataSize = NI * sizeof(Indices[0]);
    RefCntAutoPtr<IBuffer> index_buffer_handler;
    Device->CreateBuffer(idesc, &idata, &index_buffer_handler);
    return index_buffer_handler;
}
RefCntAutoPtr<ITexture> TexCube::LoadTexture(IRenderDevice* Device, const Char* TexPath)
{
    TextureLoadInfo loadInfo;
    loadInfo.IsSRGB = True;
    RefCntAutoPtr<ITexture> iTex;
    CreateTextureFromFile(TexPath, loadInfo, Device, &iTex);
    return iTex;
}
RefCntAutoPtr<IPipelineState> TexCube::CreatePipelineState(const CreateInfo& createInfo)
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PipelineStateDesc&              PSODesc          = PSOCreateInfo.PSODesc;
    PipelineResourceLayoutDesc&     ResourceLayout   = PSODesc.ResourceLayout;
    GraphicsPipelineDesc&           GraphicsPipeline = PSOCreateInfo.GraphicsPipeline;

    PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
    PSODesc.Name         = "Graphics Pipeline";

    GraphicsPipeline.NumRenderTargets             = 1;
    GraphicsPipeline.RTVFormats[0]                = createInfo.RTVFormat;
    GraphicsPipeline.DSVFormat                    = createInfo.DSVFormat;
    GraphicsPipeline.SmplDesc.Count               = createInfo.SampleCount;
    GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;
    GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

    ShaderCreateInfo ShaderCI;
    ShaderCI.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_HLSL;
    ShaderCI.Desc.UseCombinedTextureSamplers = true;

    ShaderCI.pShaderSourceStreamFactory = createInfo.pShaderSourceFactory;

    RefCntAutoPtr<IShader> vertex_shader;
    {
        ShaderCI.Desc.Name       = "vertex shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = createInfo.VSPath;
        createInfo.Device->CreateShader(ShaderCI, &vertex_shader);
    }
    RefCntAutoPtr<IShader> pixel_shader;
    {
        ShaderCI.Desc.Name       = "pixel shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = createInfo.PSPath;
        createInfo.Device->CreateShader(ShaderCI, &pixel_shader);
    }

    InputLayoutDescX InputLayout;
    Uint32           Attrib = 0;
    if (createInfo.VertexFlags & FLAG_POS)
        InputLayout.Add(Attrib++, 0u, 3u, VT_FLOAT32, False);
    if (createInfo.VertexFlags & FLAG_NORMAL)
        InputLayout.Add(Attrib++, 0u, 2u, VT_FLOAT32, False);
    if (createInfo.VertexFlags & FLAG_TEXTURE)
        InputLayout.Add(Attrib++, 0u, 2u, VT_FLOAT32, False);

    for (Uint32 i = 0; i < createInfo.NumExtraLayoutElements; ++i)
        InputLayout.Add(createInfo.ExtraLayoutElems[i]);
    GraphicsPipeline.InputLayout = InputLayout;

    PSOCreateInfo.pVS = vertex_shader;
    PSOCreateInfo.pPS = pixel_shader;

    ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    ShaderResourceVariableDesc Vars[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
    ResourceLayout.Variables    = Vars;
    ResourceLayout.NumVariables = _countof(Vars);

    SamplerDesc sampler{
        FILTER_TYPE_LINEAR,
        FILTER_TYPE_LINEAR,
        FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_CLAMP,
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP

    };
    ImmutableSamplerDesc imms[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", sampler}};

    RefCntAutoPtr<IPipelineState> pPSO;
    createInfo.Device->CreateGraphicsPipelineState(PSOCreateInfo, &pPSO);
    return pPSO;
}

void TexCube::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    LoadData();
   
    
}
void TexCube::Render()
{
}
void TexCube::Update(double ctime, double etime)
{
    SampleBase::Update(ctime, etime);
}

void TexCube::LoadData()
{
    float3 pos0(-3.0f, 0.0f, 0.0f);
    float3 pos1(3.0f, 0.0f, 0.0f);
    float3 pos2(0.0f, 2.0f, 0.0f);
    float3 pos3(0.0f, -2.0f, 0.0f);
    NV = 4;
    NI = 6;
    
    Positions.reserve(NV);
    Positions.emplace_back(pos0);
    Positions.emplace_back(pos1);
    Positions.emplace_back(pos2);
    Positions.emplace_back(pos3);

    Indices.reserve(NI);
    Indices.emplace_back(0);
    Indices.emplace_back(1);
    Indices.emplace_back(2);
    Indices.emplace_back(0);
    Indices.emplace_back(3);
    Indices.emplace_back(1);

}
}
    