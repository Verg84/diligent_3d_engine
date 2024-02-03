#include"XXX_Screen.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Framework();
    }
    void Framework::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
    }
    void Framework::Render()
    {}

    void Framework::Update(double CTime, double ETime)
    {
        SampleBase::Update(CTime, ETime);
    }

    }