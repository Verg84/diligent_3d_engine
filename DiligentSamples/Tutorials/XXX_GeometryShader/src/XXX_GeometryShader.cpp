#include"XXX_GeometryShader.hpp"

namespace Diligent
{
    SampleBase* CreateSample()
    {
        return new GeometryShader();
    }
    struct Constants
    {
        float4x4 WVP;
        float4   ViewportSize;
        float    line;
    };
    void GeometryShader::CreateUniformBuffer()
    {
        BufferDesc unifDesc;
        unifDesc.Name = "Vertex Shader Constants";
        unifDesc.BindFlags = BIND_UNIFORM_BUFFER;
        unifDesc.Usage     = USAGE_DYNAMIC;
        unifDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        unifDesc.Size           = sizeof(Constants);
        m_pDevice->CreateBuffer(unifDesc, nullptr, &Shader_Constants);
    }
    void GeometryShader::CreatePipelineState()
    {
        GraphicsPipelineStateCreateInfo pso;
        pso.PSODesc.Name = "Geometry Shader Pipeline";
        pso.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        pso.GraphicsPipeline.NumRenderTargets = 1;
        pso.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        pso.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        pso.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        pso.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
        pso.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;

        CreateUniformBuffer();

        ShaderCreateInfo ShaderCI;
        ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        ShaderCI.Desc.UseCombinedTextureSamplers = true;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> pShadrFac;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &pShadrFac);
        ShaderCI.pShaderSourceStreamFactory = pShadrFac;

        RefCntAutoPtr<IShader> pVS;
        {
            ShaderCI.Desc.Name = "vertex shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "cube.vsh";
            m_pDevice->CreateShader(ShaderCI, &pVS);
        }
        RefCntAutoPtr<IShader> pGS;
        {
            ShaderCI.Desc.Name       = "geometry shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_GEOMETRY;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "cube.gsh";
            m_pDevice->CreateShader(ShaderCI, &pGS);
        }
        RefCntAutoPtr<IShader> pPS;
        {
            ShaderCI.Desc.Name = "pixel shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "cube.psh";
        }

        LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                LayoutElement{1, 0, 2, VT_FLOAT32, False},

            };
        pso.GraphicsPipeline.InputLayout.LayoutElements = elems;
        pso.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);

        pso.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

        ShaderResourceVariableDesc Vars[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
        pso.PSODesc.ResourceLayout.Variables = Vars;
        pso.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

        SamplerDesc sampler{
            FILTER_TYPE_LINEAR,
            FILTER_TYPE_LINEAR,
            FILTER_TYPE_LINEAR,
            TEXTURE_ADDRESS_CLAMP,
            TEXTURE_ADDRESS_CLAMP,
            TEXTURE_ADDRESS_CLAMP};
        ImmutableSamplerDesc imm[] =
            {
                {SHADER_TYPE_PIXEL, "g_Texture", sampler}};
        pso.PSODesc.ResourceLayout.ImmutableSamplers = imm;
        pso.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(imm);

        m_pDevice->CreateGraphicsPipelineState(pso, &pipeline_handler);

    }
    void GeometryShader::Initialize(const SampleInitInfo &InitInfo)
    {
        SampleBase::Initialize(InitInfo);
    }
    void GeometryShader::Render()
    {

    }
    void GeometryShader::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }
    } // namespace Diligent
