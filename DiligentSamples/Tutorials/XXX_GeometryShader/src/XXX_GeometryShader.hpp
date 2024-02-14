#include"SampleBase.hpp"

namespace Diligent
{
	class GeometryShader final :public SampleBase
	{
        virtual void Initialize(const SampleInitInfo& InitInfo)override final;
        virtual void Render() override final;
        virtual void Update(double ctime, double etime) override final;
        virtual const Char* GetSampleName()
        {
            return "Geometry::Shader";
        }
	};
}