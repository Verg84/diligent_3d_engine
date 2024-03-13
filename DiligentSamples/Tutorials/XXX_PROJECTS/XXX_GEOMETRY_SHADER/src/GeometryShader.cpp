#include"GeometryShader.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new GeometryShader();
}
void GeometryShader::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
}
void GeometryShader::Update(double Ctime, double Etime)
{
    SampleBase::Update(Ctime, Etime);
}
void GeometryShader::Render()
{

}

}