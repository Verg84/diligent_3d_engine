#include"MeshShaderCurves.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new MSCurves();
    }
    void MSCurves::CreateMeshPipeline()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "MESH PIPELINE";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_MESH;
        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = false;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable         = false;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology                    = PRIMITIVE_TOPOLOGY_UNDEFINED;
        
        ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;
        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;
        sci.ShaderCompiler             = SHADER_COMPILER_DXC;
        RefCntAutoPtr<IShader> MS;
        {
            sci.Desc.Name = "MS";
            sci.Desc.ShaderType = SHADER_TYPE_MESH;
            sci.EntryPoint      = "MSMain";
            sci.FilePath        = "MS.msh";
            m_pDevice->CreateShader(sci, &MS);
        }
        RefCntAutoPtr<IShader> PS;
        {
            sci.Desc.Name = "PS";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "PSMain";
            sci.FilePath        = "PS.psh";
            m_pDevice->CreateShader(sci, &PS);
        }
        PSOCreateInfo.pMS = MS;
        PSOCreateInfo.pPS = PS;
        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
        VERIFY_EXPR(m_pPSO);

    }
    void MSCurves::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
        CreateMeshPipeline();
    }
    void MSCurves::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }
    void MSCurves::Render()
    {
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        float ClearColor[] = {0.1f, 0.1f, 0.1f, 1.0f};

        DrawMeshAttribs drm;
        drm.ThreadGroupCount = 1;
        drm.Flags            = DRAW_FLAG_VERIFY_ALL;

        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->SetPipelineState(m_pPSO);
        m_pImmediateContext->DrawMesh(drm);
    }
    }