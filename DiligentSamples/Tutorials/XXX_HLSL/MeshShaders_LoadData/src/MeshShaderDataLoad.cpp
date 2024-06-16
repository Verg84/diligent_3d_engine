#include"MeshShaderDataLoad.hpp"
#include<array>

namespace Diligent
{
    namespace
    {
        #include"../assets/structures.fxh"
    }
SampleBase* CreateSample()
{
    return new MeshShaderDataLoad();
}
void MeshShaderDataLoad::CreatePipelineMeshShader()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name = "PIPELINE with mesh shader";
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_MESH;

    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable         = false;
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology                    = PRIMITIVE_TOPOLOGY_UNDEFINED;

    ShaderCreateInfo ShaderCI;
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
    ShaderCI.Desc.UseCombinedTextureSamplers = True;
    ShaderCI.ShaderCompiler                  = SHADER_COMPILER_DXC;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    ShaderCI.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> MS;
    {
        ShaderCI.Desc.Name = "Mesh Shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_MESH;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "MS.msh";
        m_pDevice->CreateShader(ShaderCI, &MS);
    }
    RefCntAutoPtr<IShader> PS;
    {
        ShaderCI.Desc.Name = "Pixel Shadder";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint     = "main";
        ShaderCI.FilePath        = "PS.psh";
        m_pDevice->CreateShader(ShaderCI, &PS);
    }

    PSOCreateInfo.pMS = MS;
    PSOCreateInfo.pPS = PS;

    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
    VERIFY_EXPR(m_pPSO != nullptr);

    m_pPSO->CreateShaderResourceBinding(&m_SRB, true);
    VERIFY_EXPR(m_SRB != nullptr);
    
    m_SRB->GetVariableByName(SHADER_TYPE_MESH, "objectData")->Set(m_ObjectBuffer);

}
void MeshShaderDataLoad::CreateObject()
    {
    static constexpr Uint32 numVertices = 3;
    static constexpr Uint32 numIndices  = 3;

    const std::array<float3, numVertices> Positions =
        {
            float3{-0.5, 0.0, 0.0},
            float3{0.5, 0.0, 0.0},
            float3{0.0, -0.5, 0.0},

        };
    const std::array<Uint32, numIndices> Indices =
        {
            0, 1, 2
        };
    std::array<float4, numVertices> ObjectPositions{};
    for (Uint32 v = 0; v < numVertices; ++v)
    {
        ObjectPositions[v] = Positions[v];
    }
    std::array<uint4, numIndices / 3> ObjectIndices{};
    for (size_t tri = 0; tri < ObjectIndices.size(); ++tri)
    {
        const auto* src_idx{&Indices[tri * 3]};
        ObjectIndices[tri] = {src_idx[0],
                              src_idx[1],
                              src_idx[2], 0};
    }
    ObjectData Data;
    std::memcpy(Data.Positions, ObjectPositions.data(), ObjectPositions.size() * sizeof(ObjectPositions[0]));
    std::memcpy(Data.Indices, ObjectIndices.data(), ObjectIndices.size() * sizeof(ObjectIndices[0]));

    BufferDesc desc;
    desc.Name = "Vertex and Index Buffer";
    desc.Usage = USAGE_IMMUTABLE;
    desc.BindFlags = BIND_UNIFORM_BUFFER;
    desc.Size      = sizeof(Data);

    BufferData data;
    data.pData = &Data;
    data.DataSize = sizeof(Data);

    m_pDevice->CreateBuffer(desc, &data, &m_ObjectBuffer);
    VERIFY_EXPR(m_ObjectBuffer != nullptr);
    

}
void MeshShaderDataLoad::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreateObject();
    CreatePipelineMeshShader();
}
void MeshShaderDataLoad::Update(double ctime, double etime)
{
    SampleBase::Update(ctime, etime);
}
void MeshShaderDataLoad::Render()
{
    auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();
    float ClearColor[] = {0.1f, 0.1f, 0.1f, 1.0};

    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG_NONE, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    
    m_pImmediateContext->SetPipelineState(m_pPSO);
    m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    DrawMeshAttribs drm;
    drm.ThreadGroupCount = 1;
    drm.Flags            = DRAW_FLAG_VERIFY_ALL;

    m_pImmediateContext->DrawMesh(drm);

}

}