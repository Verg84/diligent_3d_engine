#include"MeshShaderTriangle.hpp"
#include<array>

namespace Diligent
{
    
	SampleBase* CreateSample()
	{
        return new MeshShaderTriangle();
    }
    void MeshShaderTriangle::CreatePipelineState()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "PIPELINE MS";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_MESH;
        
        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable         = false;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology=PRIMITIVE_TOPOLOGY_UNDEFINED;

        ShaderCreateInfo sci;
        sci.Desc.UseCombinedTextureSamplers = true;
        sci.SourceLanguage=SHADER_SOURCE_LANGUAGE_HLSL;

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
        m_SRB->GetVariableByName(SHADER_TYPE_MESH, "triangleData")->Set(m_TriangleBuffer);
        
    }
    void MeshShaderTriangle::LoadTriangle()
    {
        static constexpr Uint32 numVertices = 3;
        static constexpr Uint32 numIndices  = 3;

        std::array<float3, numVertices> Positions=
        {
            float3{-0.5, 0.0, 0.0},
            float3{0.5,0.0,0.0},
            float3{0.0,-0.5,0.0}};
        std::array<Uint32, numIndices> Indices =
        {0, 1, 2};

        std::array<float4, numVertices> TrianglePositions{};
        std::array<uint4, numIndices/3>   TriangleIndices{};

        for (Uint32 v = 0; v < numVertices; ++v)
        {
            TrianglePositions[v] = Positions[v];
        }
        for (size_t tri = 0; tri < TriangleIndices.size(); ++tri)
        {
            const auto* src_idx{&Indices[tri * 3]};
            TriangleIndices[tri] = {src_idx[0],
                                src_idx[1],
                                src_idx[2], 0};
        }
        TriangleData Data;
        std::memcpy(Data.Positions, TrianglePositions.data(), TrianglePositions.size() * sizeof(TrianglePositions[0]));
        std::memcpy(Data.Indices, TriangleIndices.data(), TriangleIndices.size() * sizeof(TriangleIndices[0]));

        BufferDesc triangleDesc;
        triangleDesc.Name = "Triangle and Index Buffers";
        triangleDesc.Usage = USAGE_IMMUTABLE;
        triangleDesc.BindFlags = BIND_UNIFORM_BUFFER;
        triangleDesc.Size      = sizeof(Data);
        BufferData triangleData;
        triangleData.pData = &Data;
        triangleData.DataSize = sizeof(Data);
        m_pDevice->CreateBuffer(triangleDesc, &triangleData, &m_TriangleBuffer);
        VERIFY_EXPR(m_TriangleBuffer != nullptr);
        
    }
    void MeshShaderTriangle::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
        LoadTriangle();
        CreatePipelineState();
    }
    void MeshShaderTriangle::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }
    void MeshShaderTriangle::Render()
    {
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        float ClearColor[] = {0.1f, 0.1f, 0.1f, 1.0f};

        DrawMeshAttribs msatrr;
        msatrr.ThreadGroupCount = 1;
        msatrr.Flags            = DRAW_FLAG_VERIFY_ALL;

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->SetPipelineState(m_pPSO);
        m_pImmediateContext->CommitShaderResources(m_SRB,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->DrawMesh(msatrr);
    }

    }