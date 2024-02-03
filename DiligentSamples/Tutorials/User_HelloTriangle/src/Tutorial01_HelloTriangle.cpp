#include"Tutorial01_HelloTriangle.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new User_HelloTriangle();
	}

	// ********************	SHADERS ***************** //
	
	// simple vertex shader that creates a procedural triangle

	static const char* VSSource = R"(
struct PSInput 
{ 
    float4 Pos   : SV_POSITION; 
    float3 Color : COLOR; 
};

void main(in  uint    VertId : SV_VertexID,
          out PSInput PSIn) 
{
    float4 Pos[3];
    Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
    Pos[1] = float4( 0.0, +0.5, 0.0, 1.0);
    Pos[2] = float4(+0.5, -0.5, 0.0, 1.0);

    float3 Col[3];
    Col[0] = float3(1.0, 0.0, 0.0); // red
    Col[1] = float3(0.0, 1.0, 0.0); // green
    Col[2] = float3(0.0, 0.0, 1.0); // blue

    PSIn.Pos   = Pos[VertId];
    PSIn.Color = Col[VertId];
}
)";

    // Pixel shader simply outputs interpolated vertex color
    static const char* PSSource = R"(
struct PSInput 
{ 
    float4 Pos   : SV_POSITION; 
    float3 Color : COLOR; 
};

struct PSOutput
{ 
    float4 Color : SV_TARGET; 
};

void main(in  PSInput  PSIn,
          out PSOutput PSOut)
{
    PSOut.Color = float4(PSIn.Color.rgb, 1.0);
}
)";


	void User_HelloTriangle::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
		
		// PIPELINE STATE OBJECT...configure all GPU stages
        GraphicsPipelineStateCreateInfo PSOCreateInfo;

		// name... can be used by the compiler
        PSOCreateInfo.PSODesc.Name = "Simple Triangle PSO";
		//pipeline type...this is a graphics pipeline
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

		// number of render targets...render to a single render target
        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
		// render target format...format of swap chain's color buffer
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = m_pSwapChain->GetDesc().ColorBufferFormat;
		// set depth buffer format...use swap chain's depth buffer
        PSOCreateInfo.GraphicsPipeline.DSVFormat = m_pSwapChain->GetDesc().DepthBufferFormat;
		// set primitive topology...primitive shapes to be rendered by the pipeline state
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		// set no back face
        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
		// disable depth testing
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;

		ShaderCreateInfo ShaderCI;
		// set system code HLSL as shader source
        ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
		// OpenGl backened textures samples
        ShaderCI.Desc.UseCombinedTextureSamplers = true;

		// Create Vertex Shader
        RefCntAutoPtr<IShader> pVS;
        {
            ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.Desc.Name       = "Triangle vertex Shader";
            ShaderCI.Source          = VSSource;
            m_pDevice->CreateShader(ShaderCI, &pVS);
		}
		// Create Pixel Shader
        RefCntAutoPtr<IShader> pPS;
		{
            ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
            ShaderCI.EntryPoint      = "main";
            ShaderCI.Desc.Name       = "Triangle Pixel Shader";
            ShaderCI.Source          = PSSource;
            m_pDevice->CreateShader(ShaderCI, &pPS);
		}

		// Create Pipeline State
        PSOCreateInfo.pVS = pVS;
        PSOCreateInfo.pPS = pPS;
        m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);
	
	}

	void User_HelloTriangle::Render()
	{
		// clear the back buffer
        const float ClearColor[] = {0.35f, 0.35f, 0.35f, 1.0};
		// engine state transitions
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		// set pipeline state in the immediate context
        m_pImmediateContext->SetPipelineState(m_pPSO);

		// commit shader resources...if any

		DrawAttribs drawAttr;
        drawAttr.NumVertices = 3; // render 3 vertices
        m_pImmediateContext->Draw(drawAttr);
	}
	void User_HelloTriangle::Update(double curTime,double elapsTime)
	{
        SampleBase::Update(curTime, elapsTime);
	}
 }