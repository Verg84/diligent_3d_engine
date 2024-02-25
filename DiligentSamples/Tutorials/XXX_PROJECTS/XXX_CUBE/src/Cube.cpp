#include"Cube.hpp"
#include "MapHelper.hpp"
#include "GraphicsUtilities.h"
namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Cube();
    }
    void Cube::CreateBuffers()
    {
        // Layout of this structure matches the one we defined in the pipeline state
        struct Vertex
        {
            float3 pos;
            float4 color;
        };

        // Cube vertices

        //      (-1,+1,+1)________________(+1,+1,+1)
        //               /|              /|
        //              / |             / |
        //             /  |            /  |
        //            /   |           /   |
        //(-1,-1,+1) /____|__________/(+1,-1,+1)
        //           |    |__________|____|
        //           |   /(-1,+1,-1) |    /(+1,+1,-1)
        //           |  /            |   /
        //           | /             |  /
        //           |/              | /
        //           /_______________|/
        //        (-1,-1,-1)       (+1,-1,-1)
        //

        // clang-format off
    Vertex CubeVerts[8] =
    {
        {float3(-1,-1,-1), float4(1,0,0,1)},
        {float3(-1,+1,-1), float4(0,1,0,1)},
        {float3(+1,+1,-1), float4(0,0,1,1)},
        {float3(+1,-1,-1), float4(1,1,1,1)},

        {float3(-1,-1,+1), float4(1,1,0,1)},
        {float3(-1,+1,+1), float4(0,1,1,1)},
        {float3(+1,+1,+1), float4(1,0,1,1)},
        {float3(+1,-1,+1), float4(0.2f,0.2f,0.2f,1)},
    };
     Uint32 Indices[] =
    {
        2,0,1, 2,3,0,
        4,6,5, 4,7,6,
        0,7,4, 0,3,7,
        1,0,4, 1,4,5,
        1,5,2, 5,6,2,
        3,6,7, 3,2,6
    };

     BufferDesc VertexBufferDesc;
     VertexBufferDesc.BindFlags=BIND_VERTEX_BUFFER;
     VertexBufferDesc.Name="Vertex Buffers";
     VertexBufferDesc.Usage=USAGE_IMMUTABLE;
     VertexBufferDesc.Size=sizeof(CubeVerts);
     BufferData vData;
     vData.pData=CubeVerts;
     vData.DataSize=sizeof(CubeVerts);
     m_pDevice->CreateBuffer(VertexBufferDesc,&vData,&Vertex_Buffer);

     BufferDesc IndexBufferDesc;
     IndexBufferDesc.BindFlags=BIND_INDEX_BUFFER;
     IndexBufferDesc.Name="Index Buffer";
     IndexBufferDesc.Usage=USAGE_IMMUTABLE;
     IndexBufferDesc.Size=sizeof(Indices);
     BufferData Idata;
     Idata.pData=Indices;
     Idata.DataSize=sizeof(Indices);
     m_pDevice->CreateBuffer(IndexBufferDesc,&Idata,&Index_Buffer);

    }
    void Cube::CreateUniformBuffer()
    {
        BufferDesc uniformBufferDesc;
        uniformBufferDesc.Name="uniform vertex buffer";
        uniformBufferDesc.BindFlags=BIND_UNIFORM_BUFFER;
        uniformBufferDesc.Usage=USAGE_DYNAMIC;
        uniformBufferDesc.Size=sizeof(float4x4);
        uniformBufferDesc.CPUAccessFlags=CPU_ACCESS_WRITE;
        m_pDevice->CreateBuffer(uniformBufferDesc,nullptr,&Uniform_Buffer);
    }
	void Cube::CreatePipelineState()
    {
        GraphicsPipelineStateCreateInfo pso;
        pso.PSODesc.Name = "Cube Graphics Pipeline";
        pso.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

		pso.GraphicsPipeline.NumRenderTargets = 1;
        pso.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        pso.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        pso.GraphicsPipeline.PrimitiveTopology =PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pso.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
        pso.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

        pso.PSODesc.ResourceLayout.DefaultVariableType=SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
		
        LayoutElement layout[]=
		{
            LayoutElement{0,0,3,VT_FLOAT32,False}
		};
		pso.GraphicsPipeline.InputLayout.LayoutElements=layout;
        pso.GraphicsPipeline.InputLayout.NumElements    = _countof(layout);

		ShaderCreateInfo psi;
        psi.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        psi.Desc.UseCombinedTextureSamplers = true;
        RefCntAutoPtr<IShaderSourceInputStreamFactory> psf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &psf);
        psi.pShaderSourceStreamFactory = psf;

		RefCntAutoPtr<IShader> vs;
		{
            psi.Desc.Name = "Vertex Shader";
			psi.Desc.ShaderType=SHADER_TYPE_VERTEX;
            psi.EntryPoint      = "main";
            psi.FilePath       = "vertex_shader_source.vsh";
            m_pDevice->CreateShader(psi, &vs);
		}

		RefCntAutoPtr<IShader> ps;
		{
            psi.Desc.Name = "Pixel Shader";
            psi.Desc.ShaderType = SHADER_TYPE_PIXEL;
            psi.EntryPoint      = "main";
            psi.FilePath        = "pixel_shader_source.psh";
            m_pDevice->CreateShader(psi, &ps);
		}

        pso.pVS = vs;
        pso.pPS = ps;

        CreateUniformBuffer();

        m_pDevice->CreateGraphicsPipelineState(pso,&pipeline_handler);

        pipeline_handler->GetStaticVariableByName(SHADER_TYPE_VERTEX,"VertexShaderConstants")->Set(Uniform_Buffer);
        pipeline_handler->CreateShaderResourceBinding(&resource_binding,true);
	}

	void Cube::Initialize(const SampleInitInfo &InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        CreatePipelineState();
        CreateBuffers();
	}

	void Cube::Render()
	{
		float ClearColor[]={1.0f,0.56f,0.2f,1.0f};
        auto* pRTV=m_pSwapChain->GetCurrentBackBufferRTV();
        auto *pDSV=m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV,ClearColor,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV,CLEAR_DEPTH_FLAG,1.0f,0,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        {
            MapHelper<float4x4> VertexConstants(m_pImmediateContext,Uniform_Buffer,MAP_WRITE,MAP_FLAG_DISCARD);
            *VertexConstants=ModelViewProj.Transpose();
        }

        const Uint64 offset=0;
        IBuffer *pbuffs[]={Vertex_Buffer};
        m_pImmediateContext->SetVertexBuffers(0,1,pbuffs,&offset,RESOURCE_STATE_TRANSITION_MODE_TRANSITION,SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(Index_Buffer,0,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        m_pImmediateContext->SetPipelineState(pipeline_handler);
        m_pImmediateContext->CommitShaderResources(resource_binding,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs indexAttrs;
        indexAttrs.IndexType=VT_UINT32;
        indexAttrs.NumIndices=36;
        indexAttrs.Flags=DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(indexAttrs);

	}
	void Cube::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
        float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(ctime) * 1.0f) * float4x4::RotationX(-PI_F * 0.1f);
        float4x4 View = float4x4::Translation(0.f, 0.0f, 8.0f);
        auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});
        auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);
        ModelViewProj = CubeModelTransform * View * SrfPreTransform * Proj;



	}
	
}