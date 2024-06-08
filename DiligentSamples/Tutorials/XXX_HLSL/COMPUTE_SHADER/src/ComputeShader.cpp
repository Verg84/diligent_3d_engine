#include "ComputeShader.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new ComputeShaderParticles();
    }
	void ComputeShaderParticles::Initialize(const SampleInitInfo &InitInfo)
	{
        SampleBase::Initialize(InitInfo);
    }
    void ComputeShaderParticles::Render()
    {}
    void ComputeShaderParticles::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }
    }