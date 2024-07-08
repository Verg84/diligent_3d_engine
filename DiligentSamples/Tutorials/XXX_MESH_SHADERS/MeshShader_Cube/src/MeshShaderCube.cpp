#include"MeshShaderCube.hpp"
#include<array>

namespace Diligent
{
    namespace
    {
        #include"../assets/structures.fxh"
    }
    SampleBase* CreateSample()
    {
        return new MeshShaderCube();
    }
    void MeshShaderCube::CreatePipelineMS()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "PIPELINE MS";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_MESH;
        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]     = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = true;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable         = false;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology                    = PRIMITIVE_TOPOLOGY_UNDEFINED;

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

        m_SRB->GetVariableByName(SHADER_TYPE_MESH, "cubeData")->Set(m_CubeBuffer);

    }
    void MeshShaderCube::LoadCubeBuffers()
    {
        static constexpr Uint32 numVertices = 24;
        static constexpr Uint32 numIndices  = 36;

        std::array<float3, numVertices> Positions =
        {
                float3{-1, -1, -1},
                float3{-1, +1, -1},
                float3{+1, +1, -1},
                float3{+1, -1, -1}, // Bottom
                float3{-1, -1, -1},
                float3{-1, -1, +1},
                float3{+1, -1, +1},
                float3{+1, -1, -1}, // Front
                float3{+1, -1, -1},
                float3{+1, -1, +1},
                float3{+1, +1, +1},
                float3{+1, +1, -1}, // Right
                float3{+1, +1, -1},
                float3{+1, +1, +1},
                float3{-1, +1, +1},
                float3{-1, +1, -1}, // Back
                float3{-1, +1, -1},
                float3{-1, +1, +1},
                float3{-1, -1, +1},
                float3{-1, -1, -1}, // Left
                float3{-1, -1, +1},
                float3{+1, -1, +1},
                float3{+1, +1, +1},
                float3{-1, +1, +1} // Top
        };
        std::array<Uint32, numIndices> Indices =
            {
                2, 0, 1, 2, 3, 0,
                4, 6, 5, 4, 7, 6,
                8, 10, 9, 8, 11, 10,
                12, 14, 13, 12, 15, 14,
                16, 18, 17, 16, 19, 18,
                20, 21, 22, 20, 22, 23
             };
        std::array<float4, numVertices> CubePositions{};
        std::array<uint4, numIndices/3>   CubeIndices{};

       for(Uint32 v=0;v<numVertices;++v)
       {
           CubePositions[v] = Positions[v];
       }
       for (size_t tri = 0; tri < CubeIndices.size(); ++tri)
       {
           const auto* src_idx{&Indices[tri * 3]};
           CubeIndices[tri] = {src_idx[0],
                               src_idx[1],
                               src_idx[2], 0};
       }
       
       CubeData Data;
       std::memcpy(Data.Positions, CubePositions.data(), CubePositions.size()* sizeof(CubePositions[0]));
       std::memcpy(Data.Indices, CubeIndices.data(), CubeIndices.size() * sizeof(CubeIndices[0]));

       BufferDesc cubeDesc;
       cubeDesc.Name = "Cube Triangles and Indexing";
       cubeDesc.Usage = USAGE_IMMUTABLE;
       cubeDesc.BindFlags = BIND_UNIFORM_BUFFER;
       cubeDesc.Size      = sizeof(Data);
       BufferData cubeData;
       cubeData.pData = &Data;
       cubeData.DataSize = sizeof(Data);

       m_pDevice->CreateBuffer(cubeDesc, &cubeData, &m_CubeBuffer);
       VERIFY_EXPR(m_CubeBuffer != nullptr);


    }
    void MeshShaderCube::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
        LoadCubeBuffers();
        CreatePipelineMS();
    }
    void MeshShaderCube::Render()
    {
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        float ClearColor[] = {0.1f, 0.1f, 0.1f, 1.0f};

        DrawMeshAttribs drm;
        drm.ThreadGroupCount = 1;
        drm.Flags=DRAW_FLAG_VERIFY_ALL;

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->SetPipelineState(m_pPSO);
        m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->DrawMesh(drm);
    }
    void MeshShaderCube::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }

}


