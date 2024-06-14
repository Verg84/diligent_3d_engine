#include"meshObject.hpp"
#include"Object.hpp"
#include"MapHelper.hpp"

namespace Diligent 
{
namespace
{
    #include"../assets//structures.fxh"
}
	SampleBase* CreateSample()
	{
        return new MObject();
	}
    void MObject::CreateConstantsBuffer()
    {
        BufferDesc cBuffer;
        cBuffer.Name = "Mesh shader constants";
        cBuffer.BindFlags = BIND_UNIFORM_BUFFER;
        cBuffer.Usage     = USAGE_DYNAMIC;
        cBuffer.CPUAccessFlags = CPU_ACCESS_WRITE;
        cBuffer.Size           = sizeof(Constants);
        m_pDevice->CreateBuffer(cBuffer, nullptr, &m_pConstants);
    }
    void MObject::CreateObject()
    {
        std::array<float4, Object::NumVertices> ObjPositions{};
        for (Uint32 v = 0; v < Object::NumVertices; ++v)
        {
            ObjPositions[v] = Object::Positions[v];
        }

        std::array<uint4, Object::NumIndices/3> ObjIndices{};
        for (size_t tri = 0; tri <ObjIndices.size(); ++tri)
        {
            const auto* src_idx{&Object::Indices[tri * 3]};
            ObjIndices[tri] = {src_idx[0],
                               src_idx[1],
                               src_idx[2],0};
        }
        ObjectData Data;

        std::memcpy(Data.Positions, ObjPositions.data(), ObjPositions.size() * sizeof(ObjPositions[0]));
        std::memcpy(Data.Indices, ObjIndices.data(), ObjIndices.size() * sizeof(ObjIndices[0]));

        BufferDesc bufDesc;
        bufDesc.Name = "Object and Index Buffer";
        bufDesc.Usage = USAGE_IMMUTABLE;
        bufDesc.BindFlags = BIND_UNIFORM_BUFFER;
        bufDesc.Size      = sizeof(Data);

        BufferData bufData;
        bufData.pData = &Data;
        bufData.DataSize = sizeof(Data);

        m_pDevice->CreateBuffer(bufDesc, &bufData, &m_ObjectBuffer);
        VERIFY_EXPR(m_ObjectBuffer != nullptr);


    }
    void MObject::CreateMeshShaderPipeline()
    {
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "PIPLEINE MESH SHADER";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_MESH;

        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 0;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FillMode = FILL_MODE_SOLID;
       // PSOCreateInfo.GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = False;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology=PRIMITIVE_TOPOLOGY_UNDEFINED;

        PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;
        
        ShaderCreateInfo ShaderCI;
        ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_GLSL;

        RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        ShaderCI.pShaderSourceStreamFactory = sf;
        ShaderCI.ShaderCompiler             = SHADER_COMPILER_DXC;
        
        

        RefCntAutoPtr<IShader> MS;
        {
            ShaderCI.Desc.Name = "Mesh Shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_MESH;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "constMS.msh";
            m_pDevice->CreateShader(ShaderCI, &MS);
            
        }

        RefCntAutoPtr<IShader> PS;
        {
            ShaderCI.Desc.Name = "Pixel Shader";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "constPS.psh";
            m_pDevice->CreateShader(ShaderCI, &PS);
           
        }
        PSOCreateInfo.pMS = MS;
        PSOCreateInfo.pPS = PS;

        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
        VERIFY_EXPR(m_pPSO != nullptr);
        m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);

        m_pSRB->GetVariableByName(SHADER_TYPE_MESH, "objData")->Set(m_ObjectBuffer);
        m_pSRB->GetVariableByName(SHADER_TYPE_MESH, "objConstants")->Set(m_pConstants);

    }
	void MObject::CreatePipeline()
	{
        GraphicsPipelineStateCreateInfo PSOCreateInfo;
        PSOCreateInfo.PSODesc.Name = "OBJ PIPELINE";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
		
		PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

		PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
		PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode=CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology=PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		ShaderCreateInfo ShaderCI;
        ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        ShaderCI.Desc.UseCombinedTextureSamplers = true;
        ShaderCI.ShaderCompiler                  = SHADER_COMPILER_DXC;


		RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr,&sf);
        ShaderCI.pShaderSourceStreamFactory = sf;

		RefCntAutoPtr<IShader> VS;
		{
            ShaderCI.Desc.Name = "VERTEX SHADER";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "VS.vsh";
            m_pDevice->CreateShader(ShaderCI, &VS);
		}

		RefCntAutoPtr<IShader> PS;
		{
            ShaderCI.Desc.Name = "PIXEL SHADER";
            ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.FilePath        = "PS.psh";
            m_pDevice->CreateShader(ShaderCI, &PS);
		}
		
		PSOCreateInfo.pVS = VS;
        PSOCreateInfo.pPS = PS;

		LayoutElement elems[] =
            {
                LayoutElement{0, 0, 3, VT_FLOAT32, False}};
        PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = elems;
        PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(elems);

		m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo,&m_pPSO);
	}
	void MObject::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        CreateObject();
        CreateConstantsBuffer();
        CreateMeshShaderPipeline();

        
		
	
        /*
        m_VertexBuffer               = Object::CreateVertexBuffer(m_pDevice, Object::VERTEX_COMPONENT_FLAG_POSITION);
        VERIFY_EXPR(m_VertexBuffer != nullptr);
		m_IndexBuffer                = Object::CreateIndexBuffer(m_pDevice);
        VERIFY_EXPR(m_IndexBuffer != nullptr);
	    */

	}
	void MObject::Render()
	{
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        // Clear the back buffer
        const float ClearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

       
        m_pImmediateContext->SetPipelineState(m_pPSO);
        m_pImmediateContext->CommitShaderResources(m_pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        {
            MapHelper<Constants> CBConstants(m_pImmediateContext, m_pConstants, MAP_WRITE, MAP_FLAG_DISCARD);
            CBConstants->ViewMat = m_ViewMat.Transpose();
            CBConstants->ViewProjMat = m_ViewProjMat.Transpose();
        }
        

        DrawMeshAttribs drawAttrs;
        drawAttrs.ThreadGroupCount = 1;
        drawAttrs.Flags            = DRAW_FLAG_VERIFY_ALL;
        drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
        m_pImmediateContext->DrawMesh(drawAttrs);
	}
	void MObject::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
        
        float4x4 RotationMatrix = float4x4::RotationY((float)ctime) * float4x4::RotationX(-PI_F * 0.1f);
        float4x4 View           = float4x4::Translation(0.f, 0.0f, 5.0f);

        auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});
        auto Proj            = GetAdjustedProjectionMatrix(m_FOV, 1.f, 1000.f);

        m_ViewMat     = RotationMatrix * View * SrfPreTransform;
        m_ViewProjMat = m_ViewMat * Proj;
	}
    }