#include"MeshShader.hpp"
#include "FastRand.hpp"
#include "AdvancedMath.hpp"
#include"MapHelper.hpp"
#include<array>

namespace Diligent
{
namespace
{
    #include"../assets/structures.fxh"
}
SampleBase* CreateSample()
{
    return new MeshShader();
}
void MeshShader::CreateMeshPipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name = "PIPLEINE";
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_MESH;
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable         = true;
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology                    = PRIMITIVE_TOPOLOGY_UNDEFINED;

    ShaderCreateInfo sci;
    sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
    sci.Desc.UseCombinedTextureSamplers = true;
    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    sci.pShaderSourceStreamFactory = sf;
    sci.ShaderCompiler=SHADER_COMPILER_DXC;
    RefCntAutoPtr<IShader> AS;
    {
        sci.Desc.Name = "AS";
        sci.Desc.ShaderType = SHADER_TYPE_AMPLIFICATION;
        sci.EntryPoint      = "main";
        sci.FilePath        = "AS.ash";
        m_pDevice->CreateShader(sci, &AS);
    }
    RefCntAutoPtr<IShader> MS;
    {
        sci.Desc.Name = "MS";
        sci.Desc.ShaderType = SHADER_TYPE_MESH;
        sci.EntryPoint      = "main";
        sci.FilePath        = "MS.msh";
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
    PSOCreateInfo.pAS = AS;
    PSOCreateInfo.pMS = MS;
    PSOCreateInfo.pPS = PS;

    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
    VERIFY_EXPR(m_pPSO != nullptr);
    m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);
    VERIFY_EXPR(m_pSRB != nullptr);

    m_pSRB->GetVariableByName(SHADER_TYPE_AMPLIFICATION, "DrawTasks")->Set(m_DrawTasksBuffer->GetDefaultView(BUFFER_VIEW_SHADER_RESOURCE));
    m_pSRB->GetVariableByName(SHADER_TYPE_AMPLIFICATION, "cbCubeData")->Set(m_CubeBuffer);
    m_pSRB->GetVariableByName(SHADER_TYPE_AMPLIFICATION, "cbConstants")->Set(m_ConstantsBuffer);
    
    m_pSRB->GetVariableByName(SHADER_TYPE_MESH, "cbCubeData")->Set(m_CubeBuffer);
    m_pSRB->GetVariableByName(SHADER_TYPE_MESH, "cbConstants")->Set(m_ConstantsBuffer);

}
void MeshShader::CreateConstantsBuffer()
    {
    BufferDesc bufdesc;
    bufdesc.Name = "Constants";
    bufdesc.Usage = USAGE_DYNAMIC;
    bufdesc.BindFlags = BIND_UNIFORM_BUFFER;
    bufdesc.CPUAccessFlags = CPU_ACCESS_WRITE;
    bufdesc.Size           = sizeof(Constants);
    m_pDevice->CreateBuffer(bufdesc, nullptr, &m_ConstantsBuffer);
    VERIFY_EXPR(m_ConstantsBuffer != nullptr);
}
void MeshShader::CreateCubeObject()
    {
    static constexpr Uint32 numVertices = 24;
    static constexpr Uint32 numIndices  = 36;

    std::array<float3, numVertices> Positions =
        {
            float3{-1, -1, -1},
            float3{-1, +1, -1},
            float3{+1, +1, -1},
            float3{+1, -1, -1},
            float3{-1, -1, -1},
            float3{-1, -1, +1},
            float3{+1, -1, +1},
            float3{+1, -1, -1},
            float3{+1, -1, -1},
            float3{+1, -1, +1},
            float3{+1, +1, +1},
            float3{+1, +1, -1},
            float3{+1, +1, -1},
            float3{+1, +1, +1},
            float3{-1, +1, +1},
            float3{-1, +1, -1},
            float3{-1, +1, -1},
            float3{-1, +1, +1},
            float3{-1, -1, +1},
            float3{-1, -1, -1},
            float3{-1, -1, +1},
            float3{+1, -1, +1},
            float3{+1, +1, +1},
            float3{-1, +1, +1}};
    std::array<Uint32, numIndices> Indices =
    {
            2, 0, 1,
            2, 3, 0,
            4, 6, 5,
            4, 7, 6,
            8, 10, 9, 
            8, 11, 10,
            12, 14, 13, 
            12, 15, 14,
            16, 18, 17, 
            16, 19, 18,
            20, 21, 22,
            20, 22, 23
    };

    std::array<float4,numVertices> CubePositions{};
    for (Uint32 v = 0; v < numVertices; ++v)
    {
        CubePositions[v] = Positions[v];
    }
    std::array<uint4, numIndices / 3> CubeIndices{};
    for (size_t tri = 0; tri < CubeIndices.size(); ++tri)
    {
        const auto* src_idx{&Indices[tri * 3]};
        CubeIndices[tri] = {src_idx[0],
                            src_idx[1],
                            src_idx[2],0};
    }
    CubeData Data;
    std::memcpy(Data.Positions, CubePositions.data(), CubePositions.size() * sizeof(CubePositions[0]));
    std::memcpy(Data.Indices, CubeIndices.data(), CubeIndices.size() * sizeof(CubeIndices[0]));

    BufferDesc bufdesc;
    bufdesc.Name = "Cube Buffers";
    bufdesc.Usage = USAGE_IMMUTABLE;
    bufdesc.BindFlags = BIND_UNIFORM_BUFFER;
    bufdesc.Size      = sizeof(Data);

    BufferData bufdata;
    bufdata.pData = &Data;
    bufdata.DataSize = sizeof(Data);
    m_pDevice->CreateBuffer(bufdesc, &bufdata, &m_CubeBuffer);
    VERIFY_EXPR(m_CubeBuffer != nullptr);

}
void MeshShader::CreateDrawTasks()
    {
    const int2 GridDim{128, 128};
    FastRandReal<float> Rnd{0, 0.0f, 1.0f};
    std::vector<DrawTask> DrawTasks;
    DrawTasks.resize(static_cast<size_t>(GridDim.x) * static_cast<size_t>(GridDim.y));

    for (int y = 0; y < GridDim.y; ++y)
    {
        for (int x = 0; x < GridDim.x; ++x)
        {
            int idx = x + y * GridDim.x;
            auto& dst = DrawTasks[idx];
            dst.BasePos.x = (x - GridDim.x / 2) * 4.0f + (Rnd() * 2.0f - 1.0f);
            dst.BasePos.y = (x - GridDim.y / 2) * 4.0f + (Rnd() * 2.0f - 1.0f);
            dst.Scale     = Rnd() * 0.5f + 0.5f;
        }
    }

    BufferDesc bufDesc;
    bufDesc.Name = "draw tasks buffer";
    bufDesc.Usage = USAGE_DEFAULT;
    bufDesc.BindFlags = BIND_SHADER_RESOURCE;
    bufDesc.Mode      = BUFFER_MODE_STRUCTURED;
    bufDesc.ElementByteStride = sizeof(DrawTasks[0]);
    bufDesc.Size              = sizeof(DrawTasks[0]) * static_cast<Uint32>(DrawTasks.size());

    BufferData bufData;
    bufData.pData = DrawTasks.data();
    bufData.DataSize = bufDesc.Size;

    m_pDevice->CreateBuffer(bufDesc, &bufData, &m_DrawTasksBuffer);
    VERIFY_EXPR(m_DrawTasksBuffer != nullptr);
}
void MeshShader::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreateDrawTasks();
    CreateCubeObject();
    CreateConstantsBuffer();
    CreateMeshPipeline();
}
void MeshShader::Update(double ctime,double etime)
{
    SampleBase::Update(ctime, etime);
}
void MeshShader::Render()
{

}
}