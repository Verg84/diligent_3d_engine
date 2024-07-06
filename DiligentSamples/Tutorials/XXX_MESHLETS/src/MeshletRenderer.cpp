#include"MeshletRenderer.hpp"
#include"MapHelper.hpp"

Diligent::MeshletRenderer::MeshletRenderer(
    RefCntAutoPtr<IEngineFactory>& engineFactory,
    RefCntAutoPtr<IRenderDevice>&  device,
    RefCntAutoPtr<ISwapChain>&     swapChain) :BaseRenderer(engineFactory, device, swapChain)
{
    CreatePipelineState();

    {
        BufferDesc BuffDesc;
        BuffDesc.Name = "Constants Buffer";
        BuffDesc.Usage = USAGE_DYNAMIC;
        BuffDesc.BindFlags = BIND_UNIFORM_BUFFER;
        BuffDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        BuffDesc.Size           = sizeof(Constants);

        m_pDevice->CreateBuffer(BuffDesc, nullptr, &m_pConstants);
        VERIFY_EXPR(m_pConstants != nullptr);
    } 
}

void Diligent::MeshletRenderer::BindScene(BaseScene* scene)
{
    m_pSRB.Release();
    m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);
    VERIFY_EXPR(m_pSRB != nullptr);

    m_pScene = scene;
    m_pMeshletCount = scene->m_pMeshletCount;

    m_pSRB->GetVariableByName(SHADER_TYPE_MESH, "_vertices")->Set(scene->m_pVertices->GetDefaultView(BUFFER_VIEW_SHADER_RESOURCE));
    m_pSRB->GetVariableByName(SHADER_TYPE_MESH, "_vertexIndices")->Set(scene->m_pVertexIndices->GetDefaultView(BUFFER_VIEW_SHADER_RESOURCE));
    m_pSRB->GetVariableByName(SHADER_TYPE_MESH, "_indices")->Set(scene->m_pIndices->GetDefaultView(BUFFER_VIEW_SHADER_RESOURCE));
    m_pSRB->GetVariableByName(SHADER_TYPE_MESH, "_meshlets")->Set(scene->m_pMeshlets->GetDefaultView(BUFFER_VIEW_SHADER_RESOURCE));
    m_pSRB->GetVariableByName(SHADER_TYPE_MESH, "_meshInfos")->Set(scene->m_pMeshInfos->GetDefaultView(BUFFER_VIEW_SHADER_RESOURCE));
    
    m_pSRB->GetVariableByName(SHADER_TYPE_AMPLIFICATION, "_meshletInfos")->Set(scene->m_pMeshInfos->GetDefaultView(BUFFER_VIEW_SHADER_RESOURCE));
    m_pSRB->GetVariableByName(SHADER_TYPE_AMPLIFICATION, "_meshInfos")->Set(scene->m_pMeshInfos->GetDefaultView(BUFFER_VIEW_SHADER_RESOURCE));
    
    m_pSRB->GetVariableByName(SHADER_TYPE_AMPLIFICATION, "_constants")->Set(m_pConstants);
    m_pSRB->GetVariableByName(SHADER_TYPE_MESH, "_constants")->Set(m_pConstants);
}

void Diligent::MeshletRenderer::Render(RefCntAutoPtr<IDeviceContext> immediateContext, float4x4 view, float4x4 viewProj)
{
    if (!isViewFrozen)
    {
        lastViewMatrix = view;
        lastViewProjMatrix = viewProj;
    }

    {
        MapHelper<Constants> CBConstants(immediateContext, m_pConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        CBConstants->ViewMat = view;
        CBConstants->ViewProjMat = viewProj;
        CBConstants->MeshletCount = m_pScene->m_pMeshletCount;
        CBConstants->MeshletCountPerInstance = m_pScene->m_pMeshletCountPerInstance;
        CBConstants->MeshCountPerInstance    = m_pScene->m_pMeshCountPerInstance;

    }

    immediateContext->SetPipelineState(m_pPSO);
    immediateContext->CommitShaderResources(m_pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    auto drawTasks = (Uint32)(m_pMeshletCount / 32);
    if (m_pMeshletCount % 32 != 0)
    {
        drawTasks++;
    }
    DrawMeshAttribs drawAttribs{drawTasks, DRAW_FLAG_VERIFY_ALL};

    immediateContext->DrawMesh(drawAttribs);    
}

void Diligent::MeshletRenderer::Print(std::ostream& out)const
{
    out << "MeshletRenderer";
}

void Diligent::MeshletRenderer::CreatePipelineState()
{
    m_pPSO->Release();

    GraphicsPipelineStateCreateInfo PSO;
    
    PipelineStateDesc &PSODesc=PSO.PSODesc;
    PSODesc.Name="Meshlet Pipeline Mesh Shader";
    PSODesc.PipelineType=PIPELINE_TYPE_MESH;

    PSO.GraphicsPipeline.NumRenderTargets = 1;
    PSO.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSO.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
    PSO.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    PSO.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
    PSO.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = false;
    PSO.GraphicsPipeline.DepthStencilDesc.DepthEnable         = true;
    PSO.GraphicsPipeline.PrimitiveTopology=PRIMITIVE_TOPOLOGY_UNDEFINED;

    PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;

    ShaderCreateInfo ShaderCI;
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_GLSL_VERBATIM;
    ShaderCI.ShaderCompiler = SHADER_COMPILER_GLSLANG;

    ShaderCI.Desc.UseCombinedTextureSamplers = true;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    ShaderCI.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> pAS;
    {
        ShaderCI.Desc.Name       = "AS";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_AMPLIFICATION;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "MeshletRenderer.task";
        m_pDevice ->CreateShader(ShaderCI, &pAS);
        VERIFY_EXPR(pAS != nullptr);
    }

    RefCntAutoPtr<IShader> pMS;
    {
        ShaderCI.Desc.Name = "MS";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_MESH;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "Meshletrenderer.mesh";
        m_pDevice->CreateShader(ShaderCI, &pMS);
        VERIFY_EXPR(pMS != nullptr);
    }

    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.Name = "PS";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "MeshletRenderer.frag";
        m_pDevice->CreateShader(ShaderCI, &pPS);
        VERIFY_EXPR(pPS != nullptr);
    }
    PSO.pAS = pAS;
    PSO.pMS = pMS;
    PSO.pPS = pPS;

    m_pDevice->CreateGraphicsPipelineState(PSO, &m_pPSO);
    VERIFY_EXPR(m_pPSO != nullptr);
    

}
