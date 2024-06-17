#include"MeshShaderConstants.hpp"
#include <array>
#include"MapHelper.hpp"

namespace Diligent
{
namespace
{
    #include"../assets/structures.fxh"
}
	SampleBase* CreateSample()
	{
        return new MeshShaderConstants();
    }
    void MeshShaderConstants::CreateConstantsBuffer()
    {
        BufferDesc constBuf;
        constBuf.Name = "Shader Constanst Buffer";
        constBuf.Usage = USAGE_DYNAMIC;
        constBuf.BindFlags = BIND_UNIFORM_BUFFER;
        constBuf.CPUAccessFlags = CPU_ACCESS_WRITE;
        constBuf.Size           = sizeof(Constants);
        m_pDevice->CreateBuffer(constBuf, nullptr, &m_ConstantsBuffer);
        VERIFY_EXPR(m_ConstantsBuffer != nullptr);
    }
    void MeshShaderConstants::CreateObject()
    {
        static const Uint32  NumVertices = 24;
        static const Uint32  NumIndices  = 36;

        std::array<float3,NumVertices> Positions
        {
            float3{-1, -1, -1}, float3{-1, +1, -1}, float3{+1, +1, -1}, float3{+1, -1, -1}, // Bottom
            float3{-1, -1, -1}, float3{-1, -1, +1}, float3{+1, -1, +1}, float3{+1, -1, -1}, // Front
            float3{+1, -1, -1}, float3{+1, -1, +1}, float3{+1, +1, +1}, float3{+1, +1, -1}, // Right
            float3{+1, +1, -1}, float3{+1, +1, +1}, float3{-1, +1, +1}, float3{-1, +1, -1}, // Back
            float3{-1, +1, -1}, float3{-1, +1, +1}, float3{-1, -1, +1}, float3{-1, -1, -1}, // Left
            float3{-1, -1, +1}, float3{+1, -1, +1}, float3{+1, +1, +1}, float3{-1, +1, +1}  // Top
        };
        std::array<Uint32, NumIndices> Indices =
            {
                2, 0, 1, 2, 3, 0,
                4, 6, 5, 4, 7, 6,
                8, 10, 9, 8, 11, 10,
                12, 14, 13, 12, 15, 14,
                16, 18, 17, 16, 19, 18,
                20, 21, 22, 20, 22, 23};
        // LOAD DATA TO BUFFERS(4-float vectors)
        std::array<float4, NumVertices> ObjectPositions{};
        for (Uint32 v = 0; v < NumVertices; ++v)
        {
            ObjectPositions[v] = Positions[v];
        }

        std::array<uint4,NumIndices / 3> ObjectIndices{};
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
        std::memcpy(Data.Indices,ObjectIndices.data(), ObjectIndices.size() * sizeof(ObjectIndices[0]));

        BufferDesc objBuf;
        objBuf.Name = "Object Vertex and Index Buffer";
        objBuf.BindFlags = BIND_UNIFORM_BUFFER;
        objBuf.Usage     = USAGE_IMMUTABLE;
        objBuf.Size      = sizeof(Data);

        BufferData objData;
        objData.pData = &Data;
        objData.DataSize = sizeof(Data);

        m_pDevice->CreateBuffer(objBuf, &objData, &m_ObjectBuffer);
        VERIFY_EXPR(m_ObjectBuffer != nullptr);
    }
    void MeshShaderConstants::CreatePipeMeshShader()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_MESH;
        PSOCreateInfo.PSODesc.Name         = "PIPELINE MESH SHADER";
        
        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_FRONT;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;

        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology=PRIMITIVE_TOPOLOGY_UNDEFINED;

        ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        sci.ShaderCompiler = SHADER_COMPILER_DXC;

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
        VERIFY_EXPR(m_SRB != nullptr);
        m_SRB->GetVariableByName(SHADER_TYPE_MESH, "objConstants")->Set(m_ConstantsBuffer);
        m_SRB->GetVariableByName(SHADER_TYPE_MESH, "objData")->Set(m_ObjectBuffer);

    }
    void MeshShaderConstants::Initialize(const SampleInitInfo &InitInfo)
    {
        SampleBase::Initialize(InitInfo);

        CreateConstantsBuffer();
        CreateObject();
        CreatePipeMeshShader();
    }
    void MeshShaderConstants::Render()
    {
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        float ClearColor[] = {0.1f, 0.1f, 0.1f, 1.0f};

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        {
            MapHelper<Constants> MSConstants(m_pImmediateContext, m_ConstantsBuffer, MAP_WRITE, MAP_FLAG_DISCARD);
            MSConstants->ViewMat = g_ViewMat.Transpose();
            MSConstants->ViewProjMat = g_ViewProjMat.Transpose();
        }

        m_pImmediateContext->SetPipelineState(m_pPSO);
        m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawMeshAttribs drm;
        drm.ThreadGroupCount=1;
        drm.Flags            = DRAW_FLAG_VERIFY_ALL;

        m_pImmediateContext->DrawMesh(drm);
    }
    void MeshShaderConstants::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
        float4x4 RotationMatrix = float4x4::RotationY((float)ctime) * float4x4::RotationX(-PI_F * 0.1f);
        float4x4 View           = float4x4::Translation(0.f, 0.0f, 5.0f);

        auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});
        auto Proj            = GetAdjustedProjectionMatrix(m_FOV, 1.f, 1000.f);

        g_ViewMat     = RotationMatrix * View * SrfPreTransform;
        g_ViewProjMat = g_ViewMat * Proj;
    }

    }