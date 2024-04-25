#include"triangle.hpp"

namespace Diligent
{
    SampleBase* CreateSample()
    {
        return new Triangle();
    }

    void Triangle::Initialize(const SampleInitInfo &InitInfo)
    {
        SampleBase::Initialize(InitInfo);

        GraphicsPipelineStateCreateInfo PSOCreateInfo;

        PSOCreateInfo.PSODesc.Name = "Simple Triangle Graphics Pipeline";
        PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

        // Set ouputs for the Pipeline to use
        PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
        PSOCreateInfo.GraphicsPipeline.RTVFormats[0]=m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        
        PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
        PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
    }
    void Triangle::Render()
    {

    }
    void Triangle::Update(double currentTime,double elapsedTime)
    {
        SampleBase::Update(currentTime, elapsedTime);
    }
 }