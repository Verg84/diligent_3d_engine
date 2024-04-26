#include"uniform_buffers.hpp"
#include "MapHelper.hpp"

namespace Diligent
{
	SampleBase *CreateSample()
	{
        return new UniformBuffers();
	}
	void UniformBuffers::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        CreatePipelineState();
        CreateBuffers();
	}
	void UniformBuffers::Render()
	{
        float ClearColor[] = {0.35f, 0.35f, 0.35f, 1.0};
        auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV,ClearColor,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG_NONE, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
         {
            // Map the buffer and write current world-view-projection matrix
            MapHelper<float4x4> CBConstants(m_pImmediateContext,Uniforms_Buffer, MAP_WRITE, MAP_FLAG_DISCARD);
            *CBConstants = MVP.Transpose();
        }

        const Uint64 offset   = 0;
        IBuffer*     pBuffs[] = {Vertex_Buffer};
        m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(Index_Buffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

         // Set the pipeline state
        m_pImmediateContext->SetPipelineState(m_pPSO);
        // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
        // makes sure that resources are transitioned to required states.
        m_pImmediateContext->CommitShaderResources(m_pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
        DrawAttrs.IndexType  = VT_UINT32; // Index type
        DrawAttrs.NumIndices = 36;
        // Verify the state of vertex and index buffers
        DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(DrawAttrs);

	}
	void UniformBuffers::Update(double c, double e)
	{
        SampleBase::Update(c, e);

         // Apply rotation
        float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(c) * 1.0f) * float4x4::RotationX(-PI_F * 0.1f);

        // Camera is at (0, 0, -5) looking along the Z axis
        float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);

        // Get pretransform matrix that rotates the scene according the surface orientation
        auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

        // Get projection matrix adjusted to the current screen orientation
        auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

        // Compute world-view-projection matrix
        MVP = CubeModelTransform * View * SrfPreTransform * Proj;
	}
    void UniformBuffers::CreateBuffers()
    {
        struct Vertex
        {
            float3 pos;
        };

        Vertex CubeVerts[8] =
            {
                {float3(-1, -1, -1)},
                {float3(-1, +1, -1)},
                {float3(+1, +1, -1)},
                {float3(+1, -1, -1)},

                {float3(-1, -1, +1)},
                {float3(-1, +1, +1)},
                {float3(+1, +1, +1)},
                {float3(+1, -1, +1)}
            };

          constexpr Uint32 Indices[] =
            {
                2, 0, 1, 2, 3, 0,
                4, 6, 5, 4, 7, 6,
                0, 7, 4, 0, 3, 7,
                1, 0, 4, 1, 4, 5,
                1, 5, 2, 5, 6, 2,
                3, 6, 7, 3, 2, 6};

        BufferDesc vdesc;
        vdesc.Usage = USAGE_IMMUTABLE;
        vdesc.BindFlags=BIND_VERTEX_BUFFER;
        vdesc.Size      = sizeof(CubeVerts);
        BufferData vdata;
        vdata.pData = CubeVerts;
        vdata.DataSize = sizeof(CubeVerts);
        m_pDevice->CreateBuffer(vdesc, &vdata, &Vertex_Buffer);

        BufferDesc idesc;
        idesc.Usage     = USAGE_IMMUTABLE;
        idesc.BindFlags = BIND_INDEX_BUFFER;
        idesc.Size      = sizeof(Indices);
        BufferData idata;
        idata.pData    = Indices;
        idata.DataSize = sizeof(Indices);
        m_pDevice->CreateBuffer(idesc, &idata, &Index_Buffer);
    }
	void UniformBuffers::CreatePipelineState()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name         = "Pipeline Uniform Buffers";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

        ShaderCreateInfo sci;
        sci.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_GLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        RefCntAutoPtr<IShader> VS;
        {
            sci.Desc.Name       = "Vertex Shader";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = "VS.glsl";
            m_pDevice->CreateShader(sci, &VS);

            BufferDesc uniformDesc;
            uniformDesc.Name = "vertex uniform buffers";
            uniformDesc.Size = sizeof(float4x4);
            uniformDesc.Usage = USAGE_DYNAMIC;
            uniformDesc.BindFlags      = BIND_UNIFORM_BUFFER;
            uniformDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
            m_pDevice->CreateBuffer(uniformDesc, nullptr, &Uniforms_Buffer);

        }
        RefCntAutoPtr<IShader> PS;
        {
            sci.Desc.Name       = "Pixel Shader";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "PS.glsl";
            m_pDevice->CreateShader(sci, &PS);
        }

        PSOCreateInfo.pVS = VS;
        PSOCreateInfo.pPS = PS;

        PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

        LayoutElement vertex_attribs[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False}};
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(vertex_attribs);
        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = vertex_attribs;

        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
        m_pPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(Uniforms_Buffer);
        m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);
    }

   

    

}