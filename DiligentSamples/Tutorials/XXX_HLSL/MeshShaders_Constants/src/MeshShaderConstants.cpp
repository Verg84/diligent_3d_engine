#include"MeshShaderConstants.hpp"

namespace Diligent
{
namespace
{
    #include"../assets/structures.fxh"
}
	SampleBase* CreateSample()
	{
        return new MeshShaderConstants();
    }
    void MeshShaderConstants::CreateConstantsBuffer()
    {
        BufferDesc constBuf;
        constBuf.Name = "Shader Constanst Buffer";
        constBuf.Usage = USAGE_DYNAMIC;
        constBuf.BindFlags = BIND_UNIFORM_BUFFER;
        constBuf.CPUAccessFlags = CPU_ACCESS_WRITE;
        constBuf.Size           = sizeof(Constants);
        m_pDevice->CreateBuffer(constBuf, nullptr, &m_ConstantsBuffer);
        VERIFY_EXPR(m_ConstantsBuffer != nullptr);
    }
    void MeshShaderConstants::Initialize(const SampleInitInfo &InitInfo)
    {
        SampleBase::Initialize(InitInfo);

        CreateConstantsBuffer();
    }
    void MeshShaderConstants::Render()
    {

    }
    void MeshShaderConstants::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }

    }