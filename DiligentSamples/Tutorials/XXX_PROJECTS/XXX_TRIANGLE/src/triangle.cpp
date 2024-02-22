#include"triangle.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Triangle();
	}
    void Triangle::CreateVertexBuffer()
    {
        struct Vertex
        {
            float3 pos;
        };
        Vertex trianglePosition[]{
            float3(-0.5,-0.5,0.0),
            float3(0.5,-0.5,0.0),
            float3(0.5,0.5,0.0)
        };
        BufferDesc vDesc;
        vDesc.Name = "Vertex Buffers";
        vDesc.BindFlags = BIND_VERTEX_BUFFER;
        vDesc.Usage     = USAGE_IMMUTABLE;
        vDesc.Size      = sizeof(trianglePosition);
        BufferData vData;
        vData.pData = trianglePosition;
        vData.DataSize = sizeof(trianglePosition);
        m_pDevice->CreateBuffer(vDesc, &vData, &vertex_buffer);
    }
    void Triangle::CreatePipelineState()
    {
        GraphicsPipelineStateCreateInfo pso;
        pso.PSODesc.Name = "Simple Triangle Graphics Pipeline";
        pso.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
        pso.GraphicsPipeline.NumRenderTargets=1;
        pso.GraphicsPipeline.RTVFormats[0]=m_pSwapChain->GetDesc().ColorBufferFormat;
        pso.GraphicsPipeline.DSVFormat       = m_pSwapChain->GetDesc().DepthBufferFormat;
        pso.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pso.GraphicsPipeline.DepthStencilDesc.DepthEnable=false;
        pso.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_NONE;

        ShaderCreateInfo sci;
        sci.Desc.UseCombinedTextureSamplers = true;
        sci.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_HLSL;
        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        RefCntAutoPtr<IShader> vertex_shader;
        {
            sci.Desc.Name = "simple triangle vertex shader";
            sci.Desc.ShaderType=SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath       = "simple_vertex_shader.vsh";       
            m_pDevice->CreateShader(sci, &vertex_shader);
        }
        RefCntAutoPtr<IShader> pixel_shader;
        {
            sci.Desc.Name = "simple triangle pixel shader";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "simple_pixel_shader.psh";
            m_pDevice->CreateShader(sci, &pixel_shader);
        }
        pso.pVS = vertex_shader;
        pso.pPS = pixel_shader;

        LayoutElement layout[] =
            {
                LayoutElement{0, 0,3, VT_FLOAT32, False}
            };
        pso.GraphicsPipeline.InputLayout.LayoutElements = layout;
        pso.GraphicsPipeline.InputLayout.NumElements    = _countof(layout);

        pso.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

        m_pDevice->CreateGraphicsPipelineState(pso, &pipeline_handler);

    }

	void Triangle::Initialize(const SampleInitInfo &InitInfo)
    {
        SampleBase::Initialize(InitInfo);
        CreatePipelineState();
        CreateVertexBuffer();
	
    }
    void Triangle::Render()
    {
        float ClearColor[] = {0.32f, 0.4f, 0.8f, 1.0};
        auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        
        const Uint64 offset = 0;
        IBuffer*      pbuffs[] = {vertex_buffer};
        m_pImmediateContext->SetVertexBuffers(0, 1, pbuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetPipelineState(pipeline_handler);
        DrawAttribs attrs;
        attrs.NumVertices = 3;
        attrs.StartVertexLocation = 0;
        m_pImmediateContext->Draw(attrs);
        
    }

    void Triangle::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }
    
}