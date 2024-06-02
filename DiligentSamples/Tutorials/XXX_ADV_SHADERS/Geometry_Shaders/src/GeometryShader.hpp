#include "SampleBase.hpp"

namespace Diligent
{
	class GeomShader final :public SampleBase
	{
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double ctime, double etime) override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Geometry Shader";
        }
	};
}