#include<vector>

#include"cube_class.hpp"
#include"BasicMath.hpp"
#include"TextureUtilities.h"
#include"GraphicsTypesX.hpp"

namespace Diligent
{
	namespace CubeClass
	{

    //      (-1,+1,+1)________________(+1,+1,+1)                  Z
    //               /|              /|                           |      Y
    //              / |             / |                           |     /
    //             /  |            /  |                           |    /
    //            /   |           /   |                           |   /
    //(-1,-1,+1) /____|__________/(+1,-1,+1)                      |  /
    //           |    |__________|____|                           | /
    //           |   /(-1,+1,-1) |    /(+1,+1,-1)                 |----------------> X
    //           |  /            |   /
    //           | /             |  /
    //           |/              | /
    //           /_______________|/
    //        (-1,-1,-1)       (+1,-1,-1)
    //

    const std::array<float3, NumVertices> Positions = {
        float3{-1, -1, -1}, float3{-1, +1, -1}, float3{+1, +1, -1}, float3{+1, -1, -1}, // Bottom
        float3{-1, -1, -1}, float3{-1, -1, +1}, float3{+1, -1, +1}, float3{+1, -1, -1}, // Front
        float3{+1, -1, -1}, float3{+1, -1, +1}, float3{+1, +1, +1}, float3{+1, +1, -1}, // Right
        float3{+1, +1, -1}, float3{+1, +1, +1}, float3{-1, +1, +1}, float3{-1, +1, -1}, // Back
        float3{-1, +1, -1}, float3{-1, +1, +1}, float3{-1, -1, +1}, float3{-1, -1, -1}, // Left
        float3{-1, -1, +1}, float3{+1, -1, +1}, float3{+1, +1, +1}, float3{-1, +1, +1}  // Top
    };

    const std::array<float2, NumVertices> Texcoords = {
        float2{0, 1}, float2{0, 0}, float2{1, 0}, float2{1, 1}, // Bottom
        float2{0, 1}, float2{0, 0}, float2{1, 0}, float2{1, 1}, // Front
        float2{0, 1}, float2{1, 1}, float2{1, 0}, float2{0, 0}, // Right
        float2{0, 1}, float2{0, 0}, float2{1, 0}, float2{1, 1}, // Back
        float2{1, 0}, float2{0, 0}, float2{0, 1}, float2{1, 1}, // Left
        float2{1, 1}, float2{0, 1}, float2{0, 0}, float2{1, 0}  // Top
    };

    const std::array<float3, NumVertices> Normals = {
        float3{0, 0, -1}, float3{0, 0, -1}, float3{0, 0, -1}, float3{0, 0, -1}, // Bottom
        float3{0, -1, 0}, float3{0, -1, 0}, float3{0, -1, 0}, float3{0, -1, 0}, // Front
        float3{+1, 0, 0}, float3{+1, 0, 0}, float3{+1, 0, 0}, float3{+1, 0, 0}, // Right
        float3{0, +1, 0}, float3{0, +1, 0}, float3{0, +1, 0}, float3{0, +1, 0}, // Back
        float3{-1, 0, 0}, float3{-1, 0, 0}, float3{-1, 0, 0}, float3{-1, 0, 0}, // Left
        float3{0, 0, +1}, float3{0, 0, +1}, float3{0, 0, +1}, float3{0, 0, +1}  // Top
    };

    // clang-format off
const std::array<Uint32, NumIndices> Indices =
{
    2,0,1,    2,3,0,
    4,6,5,    4,7,6,
    8,10,9,   8,11,10,
    12,14,13, 12,15,14,
    16,18,17, 16,19,18,
    20,21,22, 20,22,23
};
    // clang-format on

RefCntAutoPtr<IBuffer> CreateVertexBuffer(
    IRenderDevice*          pDevice,
    VERTEX_COMPONENTS_FLAGS Components,
    BIND_FLAGS              BindFlags,
    BUFFER_MODE             Mode)
{
    VERIFY_EXPR(Components != VERTEX_COMPONENT_FLAG_NONE);
    VERIFY_EXPR(Components != VERTEX_COMPONENT_FLAG_NONE);
    const Uint32 TotalVertexComponents =
        ((Components & VERTEX_COMPONENT_FLAG_POSITION) ? 3 : 0) +
        ((Components & VERTEX_COMPONENT_FLAG_NORMAL) ? 3 : 0) +
        ((Components & VERTEX_COMPONENT_FLAG_TEXCOORD) ? 2 : 0);

    std::vector<float> VertexData(size_t(TotalVertexComponents) * NumVertices);

    auto it = VertexData.begin();
    for (Uint32 v = 0; v < NumVertices; ++v)
    {
        if (Components & VERTEX_COMPONENT_FLAG_POSITION)
        {
            const auto& Pos(Positions[v]);
            *(it++) = Pos.x;
            *(it++) = Pos.y;
            *(it++) = Pos.z;
        }
        if (Components & VERTEX_COMPONENT_FLAG_NORMAL)
        {
            const auto& N(Normals[v]);
            *(it++) = N.x;
            *(it++) = N.y;
            *(it++) = N.z;
        }
        if (Components & VERTEX_COMPONENT_FLAG_TEXCOORD)
        {
            const auto& UV(TexCoords[v]);
            *(it++) = UV.x;
            *(it++) = UV.y;
        }
    }
    VERIFY_EXPR(it == VertexData.end());

    BufferDesc VertBufferDesc;
    VertBufferDesc.Name = "Cube Vertex Buffer";
    VertBufferDesc.Usage = USAGE_IMMUTABLE;
    VertBufferDesc.BindFlags = BindFlags;
    VertBufferDesc.Size      = static_cast<Uint64>(VertexData.size() * sizeof(VertexData[0]));
    VertBufferDesc.Mode      = Mode;
    if (Mode != BUFFER_MODE_UNDEFINED)
    {
        VertBufferDesc.ElementByteStride = TotalVertexComponents * sizeof(VertexData[0]);
    }
    BufferData VBData;
    VBData.pData = VertexData.data();
    VBData.DataSize = VertBufferDesc.Size;
    RefCntAutoPtr<IBuffer> pCubeVertexBuffer;

    pDevice->CreateBuffer(VertBufferDesc, &VBData, &pCubeVertexBuffer);

    return pCubeVertexBuffer;

}

RefCntAutoPtr<IBuffer> CreateIndexBuffer(IRenderDevice* pDevice, BIND_FLAGS BindFlags, BUFFER_MODE Mode)
{
    BufferDesc IndexBufDesc;
    IndexBufDesc.Name = "Cube Index Buffer";
    IndexBufDesc.Usage=USAGE_IMMUTABLE;
    IndexBufDesc.BindFlags = BindFlags;
    IndexBufDesc.Size      = sizeof(Indices);
    IndexBufDesc.Mode      = Mode;
    if (Mode != BUFFER_MODE_UNDEFINED)
        IndexBufDesc.ElementByteStride = sizeof(Indices[0]);
    BufferData IBData;
    IBData.pData = Indices.data();
    IBData.DataSize = NumIndices * sizeof(Indices[0]);
    RefCntAutoPtr<IBuffer> pBuffer;
    pDevice->CreateBuffer(IndexBufDesc, &IBData, &pBuffer);

    return pBuffer;
}

RefCntAutoPtr<ITexture> LoadTexture(IRenderDevice* pDevice, const char* Path)
{
    TextureLoadInfo loadInfo;
    loadInfo.IsSRGB=true;
    RefCntAutoPtr<ITexture> pTex;
    CreateTextureFromFile(Path, loadInfo, pDevice, &pTex);
    return pTex;
}

RefCntAutoPtr<IPipelineState> CreatePipelineState(const PSOCreateInfo& CreateInfo)
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PipelineStateDesc& PSODesc=PSOCreateInfo.PSODesc;
    PipelineResourceLayoutDesc& ResourceLayout = PSODesc.ResourceLayout;
    GraphicsPipelineDesc&       GraphicsPipeline = PSOCreateInfo.GraphicsPipeline;

    PSODesc.PipelineType=PIPELINE_TYPE_GRAPHICS;

    PSODesc.Name = "CUBE PSO";

    GraphicsPipeline.NumRenderTargets = 1;
    GraphicsPipeline.RTVFormats[0]=CreateInfo.RTVFormat;
    GraphicsPipeline.DSVFormat        = CreateInfo.DSVFormat;
    GraphicsPipeline.SmplDesc.Count   = CreateInfo.SampleCount;
    GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

    ShaderCreateInfo ShaderCI;
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_GLSL;
    ShaderCI.Desc.UseCombinedTextureSamplers = True;

    ShaderCI.pShaderSourceStreamFactory = CreateInfo.pShaderSourceFactory;

    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.Name = "CUBE VS";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "VS.glsl";
        CreateInfo.pDevice->CreateShader(ShaderCI, &pVS);
    }
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.Name = "CUBE PS";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "PS.glsl";
        CreateInfo.pDevice->CreateShader(ShaderCI, &pPS);
    }

     InputLayoutDescX InputLayout;

    Uint32 Attrib = 0;
    if (CreateInfo.Components & VERTEX_COMPONENT_FLAG_POSITION)
        InputLayout.Add(Attrib++, 0u, 3u, VT_FLOAT32, False);
    if (CreateInfo.Components & VERTEX_COMPONENT_FLAG_NORMAL)
        InputLayout.Add(Attrib++, 0u, 3u, VT_FLOAT32, False);
    if (CreateInfo.Components & VERTEX_COMPONENT_FLAG_TEXCOORD)
        InputLayout.Add(Attrib++, 0u, 2u, VT_FLOAT32, False);

    for (Uint32 i = 0; i < CreateInfo.NumExtraLayoutElements; ++i)
        InputLayout.Add(CreateInfo.ExtraLayoutElements[i]);

    GraphicsPipeline.InputLayout = InputLayout;

    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;

    // Define variable type that will be used by default
    ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    // Shader variables should typically be mutable, which means they are expected
    // to change on a per-instance basis
    // clang-format off
    ShaderResourceVariableDesc Vars[] = 
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}
    };
    // clang-format on
    ResourceLayout.Variables    = Vars;
    ResourceLayout.NumVariables = _countof(Vars);

    // Define immutable sampler for g_Texture. Immutable samplers should be used whenever possible
    // clang-format off
    SamplerDesc SamLinearClampDesc
    {
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, 
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
    };
    ImmutableSamplerDesc ImtblSamplers[] = 
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SamLinearClampDesc}
    };
    // clang-format on
    ResourceLayout.ImmutableSamplers    = ImtblSamplers;
    ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

    RefCntAutoPtr<IPipelineState> pPSO;
    CreateInfo.pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &pPSO);
    return pPSO;


}

	}
}

