#include"MeshShader.hpp"

namespace Diligent
{
void MeshShader::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
}
SampleBase* CreateSample()
{
    return new MeshShader();
}
void MeshShader::Update(double ctime, double etime)
{
    SampleBase::Update(ctime, etime);
}
void MeshShader::Render()
{

}

}