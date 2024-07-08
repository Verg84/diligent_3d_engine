#include"MeshShaderCube3d.hpp"
#include<array>
#include"MapHelper.hpp"

namespace Diligent
{
    namespace{
    #include"../assets/structures.fxh"
    }
SampleBase* CreateSample()
{
    return new MeshShaderCube3d();
}

void MeshShaderCube3d::CreateConstantsBuffer()
{
    BufferDesc constDesc;
    constDesc.Name = "Constants Buffer";
    constDesc.Usage = USAGE_DYNAMIC;
    constDesc.BindFlags = BIND_UNIFORM_BUFFER;
    constDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
    constDesc.Size           = sizeof(float4x4) * 2;
    m_pDevice->CreateBuffer(constDesc, nullptr, &m_ConstantsBuffer);
    VERIFY_EXPR(m_ConstantsBuffer != nullptr);
}
void MeshShaderCube3d::LoadObjectData()
    {
    static constexpr Uint32 numVertices = 24;
    static constexpr Uint32 numIndices = 36;

	std::array<float3, numVertices> Positions=
	{
            float3{-1, -1, -1}, float3{-1, +1, -1}, float3{+1, +1, -1}, float3{+1, -1, -1}, // Bottom
            float3{-1, -1, -1}, float3{-1, -1, +1}, float3{+1, -1, +1}, float3{+1, -1, -1}, // Front
            float3{+1, -1, -1}, float3{+1, -1, +1}, float3{+1, +1, +1}, float3{+1, +1, -1}, // Right
            float3{+1, +1, -1}, float3{+1, +1, +1}, float3{-1, +1, +1}, float3{-1, +1, -1}, // Back
            float3{-1, +1, -1}, float3{-1, +1, +1}, float3{-1, -1, +1}, float3{-1, -1, -1}, // Left
            float3{-1, -1, +1}, float3{+1, -1, +1}, float3{+1, +1, +1}, float3{-1, +1, +1}  // Top
	};
    std::array<Uint32, numIndices> Indices =
        {
            2, 0, 1, 2, 3, 0,
            4, 6, 5, 4, 7, 6,
            8, 10, 9, 8, 11, 10,
            12, 14, 13, 12, 15, 14,
            16, 18, 17, 16, 19, 18,
            20, 21, 22, 20, 22, 23};
    
    std::array<float4, numVertices> ObjectPositions{};
    std::array<uint4, numIndices / 3> ObjectIndices{};

    for (Uint32 v = 0; v < numVertices; ++v)
    {
        ObjectPositions[v] = Positions[v];
    }
    for (size_t tri = 0; tri < ObjectIndices.size(); ++tri)
    {
        const auto* src_idx{&Indices[tri * 3]};
        ObjectIndices[tri] = {
            src_idx[0],
            src_idx[1],
            src_idx[2],
            0};
    }

    ObjectData Data;
    std::memcpy(Data.Positions, ObjectPositions.data(), ObjectPositions.size() * sizeof(ObjectPositions[0]));
    std::memcpy(Data.Indices, ObjectIndices.data(), ObjectIndices.size() * sizeof(ObjectIndices[0]));

    BufferDesc bdesc;
    bdesc.Name = "Object Buffers";
    bdesc.Usage = USAGE_IMMUTABLE;
    bdesc.BindFlags = BIND_UNIFORM_BUFFER;
    bdesc.Size      = sizeof(Data);
    BufferData bdata;
    bdata.pData = &Data;
    bdata.DataSize = sizeof(Data);
    m_pDevice->CreateBuffer(bdesc, &bdata, &m_ObjectBuffer);
    VERIFY_EXPR(m_ObjectBuffer != nullptr);
	
}

void MeshShaderCube3d::CreatePipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name = "PIPELINE MS CUBE";
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
    sci.Desc.UseCombinedTextureSamplers = true;
    sci.SourceLanguage=SHADER_SOURCE_LANGUAGE_HLSL;
    
    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    sci.pShaderSourceStreamFactory = sf;
    sci.ShaderCompiler             = SHADER_COMPILER_DXC;

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

    PSOCreateInfo.pMS = MS;
    PSOCreateInfo.pPS = PS;

    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
    VERIFY_EXPR(m_pPSO != nullptr);
    m_pPSO->CreateShaderResourceBinding(&m_SRB, true);

    m_SRB->GetVariableByName(SHADER_TYPE_MESH, "objectData")->Set(m_ObjectBuffer);
    m_SRB->GetVariableByName(SHADER_TYPE_MESH, "constants")->Set(m_ConstantsBuffer);

}
void MeshShaderCube3d::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    LoadObjectData();
    CreateConstantsBuffer();
    CreatePipeline();
}
	void MeshShaderCube3d::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
        float4x4 RotationMatrix = float4x4::RotationY((float)ctime) * float4x4::RotationX(-PI_F * 0.1f);
        float4x4 View           = float4x4::Translation(0.f, 0.0f, 5.0f);

        auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});
        auto Proj            = GetAdjustedProjectionMatrix(m_FOV, 1.f, 1000.f);

        g_ViewMat     = RotationMatrix * View * SrfPreTransform;
        g_ProjViewMat = g_ViewMat * Proj;
	}
	void MeshShaderCube3d::Render()
	{
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        float ClearColor[] = {0.1f, 0.1f, 0.2f, 1.0f};
        {
            MapHelper<Constants> CBConstants(m_pImmediateContext, m_ConstantsBuffer, MAP_WRITE, MAP_FLAG_DISCARD);
            CBConstants->ViewMat = g_ViewMat.Transpose();
            CBConstants->ProjViewMat = g_ProjViewMat.Transpose();
        }
        DrawMeshAttribs dma;
        dma.ThreadGroupCount = 1;
        dma.Flags=DRAW_FLAG_VERIFY_ALL;

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->SetPipelineState(m_pPSO);
        m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->DrawMesh(dma);


	}
}