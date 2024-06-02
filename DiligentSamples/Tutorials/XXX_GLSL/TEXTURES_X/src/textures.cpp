#include"textures.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new TexturesX();
}

void TexturesX::CreatePipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name = "PIPELINE";
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]       = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_NONE;

    ShaderCreateInfo sci;
    sci.Desc.UseCombinedTextureSamplers = true;
    sci.SourceLanguage=SHADER_SOURCE_LANGUAGE_GLSL;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    sci.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> VS;
    {
        sci.Desc.Name = "Vertex Shader";
        sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
        sci.EntryPoint      = "main";
        sci.FilePath        = "VS.glsl";
        m_pDevice->CreateShader(sci, &VS);
    }

    RefCntAutoPtr<IShader> PS;
    {
        sci.Desc.Name = "Pixel Shader";
        sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
        sci.EntryPoint      = "main";
        sci.FilePath        = "PS.glsl";
        m_pDevice->CreateShader(sci, &PS);
    }

    PSOCreateInfo.pVS = VS;
    PSOCreateInfo.pPS = PS;

    LayoutElement elems[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False}};
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
}

void TexturesX::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipeline();
}

void TexturesX::Update(double c, double e)
{
    SampleBase::Update(c, e);
}

void TexturesX::Render()
{
        
}

}