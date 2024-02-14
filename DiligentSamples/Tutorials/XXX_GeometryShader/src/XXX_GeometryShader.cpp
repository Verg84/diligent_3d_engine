#include"XXX_GeometryShader.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new GeometryShader();
    }
    void GeometryShader::Initialize(const SampleInitInfo &InitInfo)
    {
        SampleBase::Initialize(InitInfo);
    }
    void GeometryShader::Render()
    {

    }
    void GeometryShader::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }
    } // namespace Diligent
