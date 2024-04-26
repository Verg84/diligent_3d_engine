#include"ass_01.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new ASS01();
    }
    void ASS01::CreateBuffers()
    {
        struct Vertex
        {
            float3 Position;
        }; 
        
        Vertex Triangle_Vertices[] =
            {
                {float3(-0.5f, -0.5f, 0.0f)},
                {float3(0.5f, -0.5f, 0.0f)},
                {float3(0.5f, 0.5f, 0.0f)}};
        Uint32 Indices[] =
            {
                0, 1, 2
            };

        BufferDesc Vdesc;
        Vdesc.Name = "Triangle Vertices";
        Vdesc.BindFlags=BIND_VERTEX_BUFFER;
        Vdesc.Usage     = USAGE_IMMUTABLE;
        Vdesc.Size     = sizeof(Triangle_Vertices);
        BufferData Vdata;
        Vdata.pData=Triangle_Vertices;
        Vdata.DataSize = sizeof(Triangle_Vertices);
        m_pDevice->CreateBuffer(Vdesc, &Vdata, &Vertex_Buffer);

        BufferDesc Idesc;
        Idesc.Name = "Triangle Indices";
        Idesc.BindFlags=BIND_INDEX_BUFFER;
        Idesc.Usage     = USAGE_IMMUTABLE;
        Idesc.Size     = sizeof(Indices);
        BufferData Idata;
        Idata.pData=Indices;
        Idata.DataSize = sizeof(Indices);
        m_pDevice->CreateBuffer(Idesc, &Idata, &Index_Buffer);

    }
	void ASS01::CreatePipelineState()
	{
        GraphicsPipelineStateCreateInfo pso;
        pso.PSODesc.Name = "ASS01 PIPELINE";

		pso.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
		pso.GraphicsPipeline.NumRenderTargets=1;
        pso.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        pso.GraphicsPipeline.DSVFormat       = m_pSwapChain->GetDesc().DepthBufferFormat;
        pso.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pso.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
        pso.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_NONE;

		ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_GLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

		RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

		RefCntAutoPtr<IShader> vertex_shader;
		{
            sci.Desc.Name = "Vertex Shader";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = "simple_vertex_shader_source.vert";
            m_pDevice->CreateShader(sci, &vertex_shader);
		}
        RefCntAutoPtr<IShader> frag_shader;
		{
            sci.Desc.Name = "Pixel Shader";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "simple_pixel_shader_source.frag";
            m_pDevice->CreateShader(sci, &frag_shader);
		}
        pso.pVS = vertex_shader;
        pso.pPS = frag_shader;

        pso.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        

        LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False}};
        pso.GraphicsPipeline.InputLayout.LayoutElements = elems;
        pso.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);
        m_pDevice->CreateGraphicsPipelineState(pso,&pipeline_handler);
        pipeline_handler->CreateShaderResourceBinding(&srb, true);

	}
	void ASS01::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        CreatePipelineState();
        CreateBuffers();
	}
	void ASS01::Render()
	{
        float ClearColor[] = {0.3f, 0.9f, 0.4f, 1.0f};
        auto* pRTV=m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV          = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        const Uint64  offsets = 0;
        IBuffer* buffer[] = {Vertex_Buffer};
        m_pImmediateContext->SetVertexBuffers(0, 1, buffer, &offsets, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(Index_Buffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        m_pImmediateContext->SetPipelineState(pipeline_handler);
        m_pImmediateContext->CommitShaderResources(srb, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs dattrs;
        dattrs.IndexType = VT_UINT32;
        dattrs.NumIndices = 3;
        dattrs.Flags=DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawIndexed(dattrs);
        	}
	void ASS01::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
	}
}