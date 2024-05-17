#include"MeshShader.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
		return new MeshShader();
	}

	void MeshShader::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
	}

	void MeshShader::Update(double c, double e)
	{
        SampleBase::Update(c, e);
	}

	void MeshShader::Render()
    {
       
	}


}