#include "Instancing.hpp"
#include <random>

#include "MapHelper.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"
#include "ColorConversion.h"
#include "imgui.h"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Instancing();
    }
    namespace
    {
    struct InstanceData
    {
        float4x4 Matrix;
    };
    } // namespace
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
    
    void Instancing::PopulateInstanceBuffer()
    {
        // Populate instance data buffer
        const auto            zGridSize = static_cast<size_t>(m_GridSize);
        std::vector<InstanceData> InstanceData(zGridSize * zGridSize * zGridSize);

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
                    auto& CurrInst = InstanceData[instId++];
                    CurrInst.Matrix        = matrix;
                    
                }
            }
        }
        // Update instance data buffer
        Uint32 DataSize = static_cast<Uint32>(sizeof(InstanceData[0]) * InstanceData.size());
        m_pImmediateContext->UpdateBuffer(instance_buffer, 0, DataSize, InstanceData.data(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    }
    void Instancing::CreateInstanceBuffer()
    {
        BufferDesc instanceDesc;
        instanceDesc.Name      = "Instances Data Buffer";
        instanceDesc.Usage     = USAGE_DEFAULT;
        instanceDesc.BindFlags = BIND_VERTEX_BUFFER;
        instanceDesc.Size      = sizeof(float4x4) * MaxGridSize;
        m_pDevice->CreateBuffer(instanceDesc, nullptr, &instance_buffer);
        PopulateInstanceBuffer();
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
        vb.Name = "Vertex Buffer";
        vb.BindFlags=BIND_VERTEX_BUFFER;
        vb.Usage     = USAGE_IMMUTABLE;
        vb.Size     = sizeof(CubeVerts);
        BufferData vd;
        vd.pData=CubeVerts;
        vd.DataSize = sizeof(CubeVerts);
        m_pDevice->CreateBuffer(vb, &vd, &vertex_buffer);

        BufferDesc idesc;
        idesc.Name = "Index Buffer";
        idesc.BindFlags=BIND_INDEX_BUFFER;
        idesc.Usage     = USAGE_IMMUTABLE;
        idesc.Size     = sizeof(Indices);
        BufferData idata;
        idata.pData=Indices;
        idata.DataSize = sizeof(Indices);
        m_pDevice->CreateBuffer(idesc, &idata, &index_buffer);
    }
    void Instancing::CreateUniformBuffer()
    {
        BufferDesc uniform_buffer_desc;
        uniform_buffer_desc.Name = "Vertex Uniform Buffer";
        uniform_buffer_desc.BindFlags=BIND_UNIFORM_BUFFER;
        uniform_buffer_desc.Usage     = USAGE_DYNAMIC;
        uniform_buffer_desc.CPUAccessFlags = CPU_ACCESS_WRITE;
        uniform_buffer_desc.Size           = sizeof(float4x4)* 2;
        m_pDevice->CreateBuffer(uniform_buffer_desc, nullptr, &uniform_buffer);
    }
	void Instancing::CreatePipelineState()
	{
        GraphicsPipelineStateCreateInfo pso;
        pso.PSODesc.Name = "Pipeline Instancing";
        pso.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
		
		pso.GraphicsPipeline.NumRenderTargets = 1;
        pso.GraphicsPipeline.RTVFormats[0]     = m_pSwapChain->GetDesc().ColorBufferFormat;
        pso.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        pso.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pso.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
        pso.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;

		ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

		RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

		RefCntAutoPtr<IShader> vertex_shader;
        {
            sci.Desc.Name = "Vertex Shader";
			sci.Desc.ShaderType=SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath       = "texture_vertex_shader_source.vsh";
            m_pDevice->CreateShader(sci, &vertex_shader);
		}
		RefCntAutoPtr<IShader> pixel_shader;
		{
            sci.Desc.Name       = "Pixel Shader";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "texture_pixel_shader_source.psh";
            m_pDevice->CreateShader(sci, &pixel_shader);
		}

        CreateUniformBuffer();


		pso.pVS = vertex_shader;
        pso.pPS = pixel_shader;

		LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                LayoutElement{1, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{2, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{3, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{4, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE}


            };
        
        pso.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        pso.GraphicsPipeline.InputLayout.LayoutElements=elems;
        pso.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);

        m_pDevice->CreateGraphicsPipelineState(pso, &pipeline_handler);
        pipeline_handler->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Vertex_Uniform_Variables")->Set(uniform_buffer);
        pipeline_handler->CreateShaderResourceBinding(&resource_binding, true);
    }
	void Instancing::Initialize(const SampleInitInfo &InitInfo)
	{
        SampleBase::Initialize(InitInfo);

        CreatePipelineState();
        CreateBuffers();
        CreateUniformBuffer();
	}
	void Instancing::Render()
	{
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        // Clear the back buffer
        float4 ClearColor = {0.350f, 0.350f, 0.350f, 1.0f};
       
        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor.Data(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        {
            // Map the buffer and write current world-view-projection matrix
            MapHelper<float4x4> CBConstants(m_pImmediateContext, uniform_buffer, MAP_WRITE, MAP_FLAG_DISCARD);
            CBConstants[0] = VP_Matrix.Transpose();
            CBConstants[1] =Rotation_Matrix.Transpose();
        }

        // Bind vertex, instance and index buffers
        const Uint64 offsets[] = {0, 0};
        IBuffer*     pBuffs[]  = {vertex_buffer,instance_buffer};
        m_pImmediateContext->SetVertexBuffers(0, _countof(pBuffs), pBuffs, offsets, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(index_buffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // Set the pipeline state
        m_pImmediateContext->SetPipelineState(pipeline_handler);
        // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
        // makes sure that resources are transitioned to required states.
        m_pImmediateContext->CommitShaderResources(resource_binding, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs DrawAttrs;       // This is an indexed draw call
        DrawAttrs.IndexType    = VT_UINT32; // Index type
        DrawAttrs.NumIndices   = 36;
        DrawAttrs.NumInstances = m_GridSize * m_GridSize * m_GridSize; // The number of instances
        // Verify the state of vertex and index buffers
        DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(DrawAttrs);
	}
	void Instancing::Update(double ctime,double etime)
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
        VP_Matrix = View * SrfPreTransform * Proj;

        // Global rotation matrix
        Rotation_Matrix = float4x4::RotationY(static_cast<float>(ctime) * 1.0f) * float4x4::RotationX(-static_cast<float>(ctime) * 0.25f);
    }
	
}