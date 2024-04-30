#include"cube.hpp"
#include"MapHelper.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new CUBE();
}

void CUBE::CreateConstantsBuffers()
{
    BufferDesc CBDesc;
    CBDesc.Name = "Uniform Buffer";
    CBDesc.Usage = USAGE_DYNAMIC;
    CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
    CBDesc.Size      = sizeof(float4x4);
    CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
    m_pDevice->CreateBuffer(CBDesc, nullptr, &ConstantsBuffer);
}

void CUBE::CreatePipeline()
{
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name = "PIPELINE CUBE";
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;

    ShaderCreateInfo ShaderCI;
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_GLSL;
    ShaderCI.Desc.UseCombinedTextureSamplers = true;

    RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
    ShaderCI.pShaderSourceStreamFactory = sf;

    RefCntAutoPtr<IShader> VS;
    {
        ShaderCI.Desc.Name = "Cube Vertex Shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "VS.glsl";
        m_pDevice->CreateShader(ShaderCI, &VS);
    }

    RefCntAutoPtr<IShader> PS;
    {
        ShaderCI.Desc.Name = "Cube Pixel Shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "PS.glsl";
        m_pDevice->CreateShader(ShaderCI, &PS);
    }

    PSOCreateInfo.pVS = VS;
    PSOCreateInfo.pPS = PS;

    CreateConstantsBuffers();


    LayoutElement vertex_attributes[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False}};
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = vertex_attributes;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(vertex_attributes);

    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
   

    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo,&m_PSO);
    m_PSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(ConstantsBuffer);


    m_PSO->CreateShaderResourceBinding(&m_SRB, true);
}

void CUBE::CreateBuffers()
{
    struct Vertex
    {
        float3 position;
    };

    Vertex CubePositions[8] =
        {
            {float3(-1, -1, -1)},
            {float3(-1, 1, -1)},
            {float3(1, 1, -1)},
            {float3(1, -1, -1)},

            {float3(-1, -1, 1)},
            {float3(-1, 1, 1)},
            {float3(1, 1, 1)},
            {float3(1, -1, 1)}};

    Uint32 Indices[] =
        {
            2, 0, 1,
            2, 3, 0,
            4, 6, 5,
            4, 7, 6,
            0, 7, 4,
            0, 3, 7,
            1, 0, 4,
            1, 4, 5,
            1, 5, 2,
            5, 6, 2,
            3, 6, 7,
            3, 2, 6};

    BufferDesc vdesc;
    vdesc.Name = "Cube Vertices";
    vdesc.Usage = USAGE_IMMUTABLE;
    vdesc.BindFlags = BIND_VERTEX_BUFFER;
    vdesc.Size      = sizeof(CubePositions);

    BufferData vdata;
    vdata.pData = CubePositions;
    vdata.DataSize = sizeof(CubePositions);

    m_pDevice->CreateBuffer(vdesc, &vdata, &VBuf);

    BufferDesc idesc;
    idesc.Name = "Cube Indices";
    idesc.Usage = USAGE_IMMUTABLE;
    idesc.BindFlags = BIND_INDEX_BUFFER;
    idesc.Size      = sizeof(Indices);

    BufferData idata;
    idata.pData = Indices;
    idata.DataSize = sizeof(Indices);

    m_pDevice->CreateBuffer(idesc, &idata, &IBuf);
}

void CUBE::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipeline();
    CreateBuffers();
    
}

void CUBE::Render()
{
    float ClearColor[] = {1.0f, 0.0f, 0.0f, 1.0};
    auto* pRTV         = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

    m_pImmediateContext->ClearRenderTarget(
        pRTV,
        ClearColor,
        RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    m_pImmediateContext->ClearDepthStencil(
        pDSV,
        CLEAR_DEPTH_FLAG,
        1.0f,
        0,
        RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        MapHelper<float4x4> CBConstants(
            m_pImmediateContext,
            ConstantsBuffer,
            MAP_WRITE,
            MAP_FLAG_DISCARD);
        *CBConstants = MVP.Transpose();
    }

   const Uint64 offset = 0;
   IBuffer*     pbuf[] = {VBuf};
   
   m_pImmediateContext->SetVertexBuffers(
       0,
       1,
       pbuf,
       &offset,
       RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
       SET_VERTEX_BUFFERS_FLAG_RESET);
   
   m_pImmediateContext->SetIndexBuffer(
       IBuf,
       0,
       RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
   
   m_pImmediateContext->SetPipelineState(m_PSO);
   m_pImmediateContext->CommitShaderResources(m_SRB,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

   DrawIndexedAttribs dr;
   dr.IndexType = VT_UINT32;
   dr.NumIndices = 36;
   dr.Flags      = DRAW_FLAG_VERIFY_ALL;

   m_pImmediateContext->DrawIndexed(dr);

}

void CUBE::Update(double c, double e)
{
    SampleBase::Update(c, e);

    // Apply rotation
    float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(c) * 1.0f) * float4x4::RotationX(-PI_F * 0.1f);

    // Camera is at (0, 0, -5) looking along the Z axis
    float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);

    // Get pretransform matrix that rotates the scene according the surface orientation
    auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

    // Get projection matrix adjusted to the current screen orientation
    auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

    // Compute world-view-projection matrix
    MVP = CubeModelTransform * View * SrfPreTransform * Proj;
}

}