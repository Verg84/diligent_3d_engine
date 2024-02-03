#include "XXX_Functions.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Functions();
    }
    void Functions::LoadShaders(const Char* vsrc, const Char* psrc)
    {
        ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;
        RefCntAutoPtr<IShaderSourceInputStreamFactory> psf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &psf);
        sci.pShaderSourceStreamFactory = psf;
        {
            sci.Desc.Name = "vertex shader";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = vsrc;
            m_pDevice->CreateShader(sci, &vertex_shader);
        }
        {
            sci.Desc.Name = "pixel shader";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = psrc;
            m_pDevice->CreateShader(sci, &pixel_shader);
        }
        

    }
    void Functions::PipelineStateInit()
    {
        GraphicsPipelineStateCreateInfo psi;
        psi.PSODesc.Name         = "Graphics Pipeline";
        psi.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        GraphicsPipelineDesc& graphs = psi.GraphicsPipeline;
        graphs.NumRenderTargets      = 1;
        graphs.RTVFormats[0]         = m_pSwapChain->GetDesc().ColorBufferFormat;
        graphs.DSVFormat             = m_pSwapChain->GetDesc().DepthBufferFormat;
        graphs.PrimitiveTopology     = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        DepthStencilStateDesc& depth = graphs.DepthStencilDesc;
        depth.DepthEnable            = true;
        RasterizerStateDesc& rast    = graphs.RasterizerDesc;
        rast.CullMode                = CULL_MODE_BACK;

        LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                LayoutElement{1, 0, 2, VT_FLOAT32, False}};
        InputLayoutDesc& layout = graphs.InputLayout;
        layout.LayoutElements   = elems;
        layout.NumElements      = _countof(elems);

       
        psi.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
        PipelineResourceLayoutDesc res = psi.PSODesc.ResourceLayout;
        ShaderResourceVariableDesc vars[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture",SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}
            };
        SamplerDesc sampler
        {
            FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
            TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};
        ImmutableSamplerDesc immut[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture",sampler}
            };
        res.Variables = vars;
        res.NumVariables = _countof(vars);
        res.ImmutableSamplers = immut;
        res.NumImmutableSamplers = _countof(immut);

        LoadShaders("vertex_shader_source.vsh", "pixel_shader_source.psh");
        psi.pVS = vertex_shader;
        psi.pPS = pixel_shader;

    }
    void Functions::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
        PipelineStateInit();
    }
    void Functions::Render()
    {

    }
    void Functions::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }

    }