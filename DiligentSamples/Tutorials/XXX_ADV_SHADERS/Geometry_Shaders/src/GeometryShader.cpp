#include"GeometryShader.hpp"

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
    void GeomShader::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }
    void GeomShader::Render()
    {

    }

    }