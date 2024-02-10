#include"Mesh.hpp"

namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Mesh();
    }
    void Mesh::Initialize(const SampleInitInfo& InitInfo)
    {
        SampleBase::Initialize(InitInfo);
    }
    void Mesh::Render()
    {

    }
    void Mesh::Update(double ctime, double etime)
    {
        SampleBase::Update(ctime, etime);
    }


    }


