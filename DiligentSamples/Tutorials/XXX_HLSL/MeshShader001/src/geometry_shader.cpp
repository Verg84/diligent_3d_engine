#include"geometry_shader.hpp"

namespace Diligent
{
    SampleBase* CreateSample()
    {
        return new GeomShader();
    }
    
    void GeomShader::CreatePipeline()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.PipelineType=PIPELINE_TYPE_GRAPHICS;
        PSOCreateInfo.PSODesc.Name="PIPELINE PASS THROUGH GEOM> SHADER";

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_NONE;

        ShaderCreateInfo sci;
        sci.Desc.UseCombinedTextureSamplers = True;
        sci.SourceLanguage=SHADER_SOURCE_LANGUAGE_GLSL;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;
        
        RefCntAutoPtr<IShader> VS;
        {
            sci.Desc.Name = "VS";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = "VS.glsl";
            m_pDevice->CreateShader(sci, &VS);
        }
        RefCntAutoPtr<IShader> GS;
        {
            sci.Desc.Name = "GS";
            sci.Desc.ShaderType = SHADER_TYPE_GEOMETRY;
            sci.EntryPoint      = "main";
            sci.FilePath        = "GS.glsl";
            m_pDevice->CreateShader(sci, &GS);
        }
        RefCntAutoPtr<IShader> PS;
        {
            sci.Desc.Name = "PS";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "PS.glsl";
            m_pDevice->CreateShader(sci, &PS);
        }

        PSOCreateInfo.pVS = VS;
        PSOCreateInfo.pGS = GS;
        PSOCreateInfo.pPS = PS;

        LayoutElement elems[] =
            {
                LayoutElement{0, 0, 0, VT_FLOAT32, False}};
        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);

        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_PSO);
    }
	void GeomShader::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
    }

    void GeomShader::Update(double c, double e)
    {
        SampleBase::Update(c, e);
    }

    void GeomShader::Render()
    {

    }
    }