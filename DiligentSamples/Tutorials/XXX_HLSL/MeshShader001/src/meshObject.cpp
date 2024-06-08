#include"meshObject.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new MObject();
	}
	void MObject::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
	}
	void MObject::Render()
	{

	}
	void MObject::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
	}
    }