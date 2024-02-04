#include"XXX_Instancing.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"
#include "ColorConversion.h"
namespace Diligent
{
	SampleBase *CreateSample()
	{
        return new Instancing();
    }
    void Instancing::CreateUniformBuffer()
    {
        BufferDesc ubd;
        ubd.Name = "Vertex Constants - Matrices";
        ubd.BindFlags = BIND_UNIFORM_BUFFER;
        ubd.Usage     = USAGE_DYNAMIC;
        ubd.CPUAccessFlags = CPU_ACCESS_WRITE;
        ubd.Size           = sizeof(float4x4) * 2;
        m_pDevice->CreateBuffer(ubd, nullptr, &uniform_buffer);
    }
    void Instancing::Loadtexture()
    {
        // Texture Loader 
        TextureLoadInfo texload;
        texload.IsSRGB = true;
        RefCntAutoPtr<ITexture> tex;
        CreateTextureFromFile("DGLogo.png", texload, m_pDevice, &tex);
        // Get Shader resource view from the texture
        texture_handler = tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
        // set texture shaer resource to binder
        resource_binding->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(texture_handler);
    }
    void Instancing::LoadShaders(const Char* vertex_shader_src, const Char* pixel_shader_src, GraphicsPipelineStateCreateInfo& psi)
    {
        ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        {
            sci.Desc.Name = "Vertex Shader::INSTANCING";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = vertex_shader_src;
            m_pDevice->CreateShader(sci, &vertex_shader_handler);
        }
        {
            sci.Desc.Name = "Pixel Shader::INSTANCING";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = pixel_shader_src;
            m_pDevice->CreateShader(sci, &pixel_shader_handler);
        }
       
        InputLayoutDesc& layout = psi.GraphicsPipeline.InputLayout;
        LayoutElement    elems[] =
            {
                // Buffer Slot 0 //

                // ... attribute 0
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                // ...attribute 1
                LayoutElement{1, 0, 2, VT_FLOAT32, False},

                // Buffer Slot 1 //

                // ...attrib 2
                LayoutElement{2, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                // ...attrib 3
                LayoutElement{3, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{4, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{5, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
            };
    
        layout.LayoutElements =elems;
        layout.NumElements    = _countof(elems);

        CreateUniformBuffer();
        psi.pVS = vertex_shader_handler;
        psi.pPS = pixel_shader_handler;


        psi.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        ShaderResourceVariableDesc vars[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
        psi.PSODesc.ResourceLayout.Variables    = vars;
        psi.PSODesc.ResourceLayout.NumVariables = _countof(vars);
        SamplerDesc sampler{
            FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};  
        ImmutableSamplerDesc immutables[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture", sampler}};
        psi.PSODesc.ResourceLayout.ImmutableSamplers=immutables;
        psi.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(immutables);
        
    }
    void Instancing::CreatePipelineParams(GraphicsPipelineStateCreateInfo &psi)
	{
       
        psi.PSODesc.Name = "GRAPHICS PIPELINE::INSTANCING XXX";
        psi.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
		// graphics 
        GraphicsPipelineDesc& graphs = psi.GraphicsPipeline;
        graphs.NumRenderTargets      = 1;
        graphs.RTVFormats[0]         = m_pSwapChain->GetDesc().ColorBufferFormat;
        graphs.DSVFormat             = m_pSwapChain->GetDesc().DepthBufferFormat;
        graphs.PrimitiveTopology     = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		// depth-stencil state
        DepthStencilStateDesc& depth = graphs.DepthStencilDesc;
        depth.DepthEnable            = true;
        RasterizerStateDesc& rast    = graphs.RasterizerDesc;
        rast.CullMode                = CULL_MODE_BACK;


       
	}
    void Instancing::PipelineStateInit(const Char* vsrc, const Char* psrc)
    {
        GraphicsPipelineStateCreateInfo psi;
        CreatePipelineParams(psi);
        LoadShaders(vsrc,psrc, psi);
        m_pDevice->CreateGraphicsPipelineState(psi, &pipeline_handler);
        pipeline_handler->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants");

        pipeline_handler->CreateShaderResourceBinding(&resource_binding, true);

    }

    void Instancing::CreateVertexBuffer()
    {
        BufferDesc vdesc;
        vdesc.Name = "VERTEX POSITIONS BUFFER";
        vdesc.Usage = USAGE_IMMUTABLE;
        vdesc.BindFlags = BIND_VERTEX_BUFFER;
        vdesc.Size      = sizeof(CubeVerts);
        BufferData vdata;
        vdata.pData = CubeVerts;
        vdata.DataSize = sizeof(CubeVerts);
        m_pDevice->CreateBuffer(vdesc, &vdata, &vertex_buffer);
    }
    void Instancing::CreateIndexBuffer()
    {
        BufferDesc indexDesc;
        indexDesc.Name = "INDEXED POSITIONS BUFFER";
        indexDesc.Usage = USAGE_IMMUTABLE;
        indexDesc.BindFlags = BIND_INDEX_BUFFER;
        indexDesc.Size      = sizeof(Indices);
        BufferData indexData;
        indexData.pData = Indices;
        indexData.DataSize = sizeof(Indices);
        m_pDevice->CreateBuffer(indexDesc, &indexData, &index_buffer);

    }
   
    void Instancing::CreatePipelineState()
    {
        GraphicsPipelineStateCreateInfo psi;
        psi.PSODesc.Name         = "GRAPHICS PIPELINE::INSTANCING XXX";
        psi.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
        // graphics
        GraphicsPipelineDesc& graphs = psi.GraphicsPipeline;
        graphs.NumRenderTargets      = 1;
        graphs.RTVFormats[0]         = m_pSwapChain->GetDesc().ColorBufferFormat;
        graphs.DSVFormat             = m_pSwapChain->GetDesc().DepthBufferFormat;
        graphs.PrimitiveTopology     = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        // depth-stencil state
        DepthStencilStateDesc& depth = graphs.DepthStencilDesc;
        depth.DepthEnable            = true;
        RasterizerStateDesc& rast    = graphs.RasterizerDesc;
        rast.CullMode                = CULL_MODE_BACK;

        ShaderCreateInfo sci;
        sci.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

        {
            sci.Desc.Name       = "Vertex Shader::INSTANCING";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = "cube_inst.vsh";
            m_pDevice->CreateShader(sci, &vertex_shader_handler);
        }
        {
            sci.Desc.Name       = "Pixel Shader::INSTANCING";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "cube_inst_psh";
            m_pDevice->CreateShader(sci, &pixel_shader_handler);
        }

        InputLayoutDesc& layout = psi.GraphicsPipeline.InputLayout;
        LayoutElement    elems[] =
            {
                // Buffer Slot 0 //

                // ... attribute 0
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                // ...attribute 1
                LayoutElement{1, 0, 2, VT_FLOAT32, False},

                // Buffer Slot 1 //

                // ...attrib 2
                LayoutElement{2, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                // ...attrib 3
                LayoutElement{3, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{4, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{5, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
            };

        layout.LayoutElements = elems;
        layout.NumElements    = _countof(elems);

        CreateUniformBuffer();
        psi.pVS = vertex_shader_handler;
        psi.pPS = pixel_shader_handler;


        psi.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        ShaderResourceVariableDesc vars[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
        psi.PSODesc.ResourceLayout.Variables    = vars;
        psi.PSODesc.ResourceLayout.NumVariables = _countof(vars);
        SamplerDesc sampler{
            FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};
        ImmutableSamplerDesc immutables[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture", sampler}};
        psi.PSODesc.ResourceLayout.ImmutableSamplers    = immutables;
        psi.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(immutables);

        m_pDevice->CreateGraphicsPipelineState(psi, &pipeline_handler);
        pipeline_handler->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants");

        pipeline_handler->CreateShaderResourceBinding(&resource_binding, true);
    }
	void Instancing::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        CreatePipelineState();
        CreateVertexBuffer();
        CreateIndexBuffer();
        Loadtexture();
	}
	void Instancing::Render()
	{

	}
	void Instancing::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
	}

    }