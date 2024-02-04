#include"XXX_Instancing.hpp"

namespace Diligent
{
	SampleBase *CreateSample()
	{
        return new Instancing();
    }
	void Instancing::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
	}
	void Instancing::Render()
	{

	}
	void Instancing::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
	}

    }