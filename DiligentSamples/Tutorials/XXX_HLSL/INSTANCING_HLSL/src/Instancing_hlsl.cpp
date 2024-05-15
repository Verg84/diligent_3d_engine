#include "Instancing_hlsl.hpp"
#include <random>
#include "MapHelper.hpp"
#include "imgui.h"

namespace Diligent
{
    SampleBase* CreateSample()
    {
        return new Instancing();
    }
    void Instancing::CreateBuffers()
    {
        struct Vertex
        {
            float3 Pos;
        };

        Vertex CubeVerts[] =
            {
                {float3(-1, -1, -1)},
                {float3(-1, +1, -1)},
                {float3(+1, +1, -1)},
                {float3(+1, -1, -1)},

                {float3(-1, -1, -1)},
                {float3(-1, -1, +1)},
                {float3(+1, -1, +1)},
                {float3(+1, -1, -1)},

                {float3(+1, -1, -1)},
                {float3(+1, -1, +1)},
                {float3(+1, +1, +1)},
                {float3(+1, +1, -1)},

                {float3(+1, +1, -1)},
                {float3(+1, +1, +1)},
                {float3(-1, +1, +1)},
                {float3(-1, +1, -1)},

                {float3(-1, +1, -1)},
                {float3(-1, +1, +1)},
                {float3(-1, -1, +1)},
                {float3(-1, -1, -1)},

                {float3(-1, -1, +1)},
                {float3(+1, -1, +1)},
                {float3(+1, +1, +1)},
                {float3(-1, +1, +1)}};

        Uint32 Indices[] =
            {
                2, 0, 1, 2, 3, 0,
                4, 6, 5, 4, 7, 6,
                8, 10, 9, 8, 11, 10,
                12, 14, 13, 12, 15, 14,
                16, 18, 17, 16, 19, 18,
                20, 21, 22, 20, 22, 23};

        BufferDesc vb;
        vb.Name      = "Vertex Buffer";
        vb.BindFlags = BIND_VERTEX_BUFFER;
        vb.Usage     = USAGE_IMMUTABLE;
        vb.Size      = sizeof(CubeVerts);
        BufferData vd;
        vd.pData    = CubeVerts;
        vd.DataSize = sizeof(CubeVerts);
        m_pDevice->CreateBuffer(vb, &vd, &m_Vertex_Buffer);

        BufferDesc idesc;
        idesc.Name      = "Index Buffer";
        idesc.BindFlags = BIND_INDEX_BUFFER;
        idesc.Usage     = USAGE_IMMUTABLE;
        idesc.Size      = sizeof(Indices);
        BufferData idata;
        idata.pData    = Indices;
        idata.DataSize = sizeof(Indices);
        m_pDevice->CreateBuffer(idesc, &idata, &m_Index_Buffer);
    
    }
    void Instancing::PopulateInstanceBuffer()
    {
        // Populate instance data buffer
        const auto            zGridSize = static_cast<size_t>(m_GridSize);
        std::vector<float4x4> InstanceData(zGridSize * zGridSize * zGridSize);

        float fGridSize = static_cast<float>(m_GridSize);

        std::mt19937 gen; // Standard mersenne_twister_engine. Use default seed
                          // to generate consistent distribution.

        std::uniform_real_distribution<float> scale_distr(0.3f, 1.0f);
        std::uniform_real_distribution<float> offset_distr(-0.15f, +0.15f);
        std::uniform_real_distribution<float> rot_distr(-PI_F, +PI_F);

        float BaseScale = 0.6f / fGridSize;
        int   instId    = 0;
        for (int x = 0; x < m_GridSize; ++x)
        {
            for (int y = 0; y < m_GridSize; ++y)
            {
                for (int z = 0; z < m_GridSize; ++z)
                {
                    // Add random offset from central position in the grid
                    float xOffset = 2.f * (x + 0.5f + offset_distr(gen)) / fGridSize - 1.f;
                    float yOffset = 2.f * (y + 0.5f + offset_distr(gen)) / fGridSize - 1.f;
                    float zOffset = 2.f * (z + 0.5f + offset_distr(gen)) / fGridSize - 1.f;
                    // Random scale
                    float scale = BaseScale * scale_distr(gen);
                    // Random rotation
                    float4x4 rotation = float4x4::RotationX(rot_distr(gen));
                    rotation *= float4x4::RotationY(rot_distr(gen));
                    rotation *= float4x4::RotationZ(rot_distr(gen));
                    // Combine rotation, scale and translation
                    float4x4 matrix        = rotation * float4x4::Scale(scale, scale, scale) * float4x4::Translation(xOffset, yOffset, zOffset);
                    InstanceData[instId++] = matrix;
                }
            }
        }
        // Update instance data buffer
        Uint32 DataSize = static_cast<Uint32>(sizeof(InstanceData[0]) * InstanceData.size());
        m_pImmediateContext->UpdateBuffer(m_Instance, 0, DataSize, InstanceData.data(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }
    
    void Instancing::CreateInstanceBuffer()
    {
        BufferDesc InstBufDesc;
        InstBufDesc.Name = "Intance Data Buffer";
        InstBufDesc.Usage=USAGE_DEFAULT;
        InstBufDesc.BindFlags = BIND_VERTEX_BUFFER;
        InstBufDesc.Size      = sizeof(float4x4)*MaxInstances;
        m_pDevice->CreateBuffer(InstBufDesc, nullptr, &m_Instance);
        PopulateInstanceBuffer();
    }
    void Instancing::CreateUniformBuffer()
    {
        BufferDesc uniformBuf;
        uniformBuf.Name = "Constants Buffer";
        uniformBuf.Usage = USAGE_DYNAMIC;
        uniformBuf.BindFlags = BIND_UNIFORM_BUFFER;
        uniformBuf.Size      = sizeof(float4x4) * 2;
        uniformBuf.CPUAccessFlags = CPU_ACCESS_WRITE;
        m_pDevice->CreateBuffer(uniformBuf, nullptr, &m_Constants);
    }
    void Instancing::CreatePipeline()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "PIPELINE INSTANCING";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

        ShaderCreateInfo ShaderCI;
        ShaderCI.Desc.UseCombinedTextureSamplers = true;
        ShaderCI.SourceLanguage=SHADER_SOURCE_LANGUAGE_HLSL;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        ShaderCI.pShaderSourceStreamFactory = sf;

        RefCntAutoPtr<IShader> VS;
        {
            ShaderCI.Desc.Name = "Instancing Vertex Shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "cube_inst.vsh";
            m_pDevice->CreateShader(ShaderCI, &VS);
        }

        RefCntAutoPtr<IShader> PS;
        {
            ShaderCI.Desc.Name = "Instancing Pixel Shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
            ShaderCI.EntryPoint       = "main";
            ShaderCI.FilePath        = "cube_inst.psh";
            m_pDevice->CreateShader(ShaderCI, &PS);
        }

        PSOCreateInfo.pVS = VS;
        PSOCreateInfo.pPS = PS;

        LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                LayoutElement{1, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{2, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{3, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{4, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE}};
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(elems);
        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;

        PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType=SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
        CreateUniformBuffer();

        m_pPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_Constants);
        m_pPSO->CreateShaderResourceBinding(&m_SRB, true);
    }
    void Instancing::UpdateUI()
    {
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::SliderInt("Grid Size", &m_GridSize, 1, 32))
            {
                PopulateInstanceBuffer();
            }
        }
        ImGui::End();
    }

	void Instancing::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);

        
        CreatePipeline();
        CreateBuffers();
        CreateInstanceBuffer();
    }
    void Instancing::Render()
    {
        float ClearColor[] = {0.35f, 0.35f, 0.5f, 1.0f};
        auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        {
            MapHelper<float4x4> CBConstants(m_pImmediateContext, m_Constants, MAP_WRITE,MAP_FLAG_DISCARD);
            CBConstants[0] = m_ViewProjMatrix.Transpose();
            CBConstants[1] = m_RotationMatrix.Transpose();
        }

        const Uint64 offsets[] = {0, 0};
        IBuffer*     pbuffers[] = {m_Vertex_Buffer, m_Instance};
        m_pImmediateContext->SetVertexBuffers(0, _countof(pbuffers), pbuffers, offsets, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(m_Index_Buffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // Set the pipeline state
        m_pImmediateContext->SetPipelineState(m_pPSO);
        // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
        // makes sure that resources are transitioned to required states.
        m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs DrawAttrs;       // This is an indexed draw call
        DrawAttrs.IndexType    = VT_UINT32; // Index type
        DrawAttrs.NumIndices   = 36;
        DrawAttrs.NumInstances = m_GridSize * m_GridSize * m_GridSize; // The number of instances
        // Verify the state of vertex and index buffers
        DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(DrawAttrs);
    }
    void Instancing::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);

        UpdateUI();

        // Set cube view matrix
        float4x4 View = float4x4::RotationX(-0.6f) * float4x4::Translation(0.f, 0.f, 4.0f);

        // Get pretransform matrix that rotates the scene according the surface orientation
        auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

        // Get projection matrix adjusted to the current screen orientation
        auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

        // Compute view-projection matrix
        m_ViewProjMatrix = View * SrfPreTransform * Proj;

        // Global rotation matrix
        m_RotationMatrix = float4x4::RotationY(static_cast<float>(ctime) * 1.0f) * float4x4::RotationX(-static_cast<float>(ctime) * 0.25f);
    }
    
    

}