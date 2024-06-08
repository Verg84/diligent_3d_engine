#include"meshObject.hpp"
#include"Object.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new MObject();
	}
	void MObject::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        
		
		RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        Object::CreatePSOInfo PSOInfo;
        PSOInfo.pShaderSourceFactory = sf;
		PSOInfo.Components=Object::VERTEX_COMPONENT_FLAG_POSITION;
		PSOInfo.pDevice=m_pDevice;
        PSOInfo.RTVFormat            = m_pSwapChain->GetDesc().ColorBufferFormat;
        PSOInfo.DSVFormat            = m_pSwapChain->GetDesc().DepthBufferFormat;
        PSOInfo.VSFilePath           = "VS.vsh";
        PSOInfo.PSFilePath           = "PS.psh";
        m_pPSO                       = Object::CreatePipelineState(PSOInfo);
        VERIFY_EXPR(m_pPSO != nullptr);
        m_VertexBuffer               = Object::CreateVertexBuffer(m_pDevice, Object::VERTEX_COMPONENT_FLAG_POSITION);
        m_IndexBuffer                = Object::CreateIndexBuffer(m_pDevice);

			

	}
	void MObject::Render()
	{

	}
	void MObject::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
	}
    }