#include"geometry_shader.hpp"

namespace Diligent
{
SampleBase* CreateSample()
{
    return new GeomShader();
}
	void GeomShader::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
    }

    void GeomShader::Update(double c, double e)
    {
        SampleBase::Update(c, e);
    }

    void GeomShader::Render()
    {

    }
    }