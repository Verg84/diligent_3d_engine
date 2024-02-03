#include"SampleBase.hpp"

namespace Diligent
{
	class Framework final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo)override final;
        virtual void Update(double CTime, double ETime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName() const override final
        {
            return "***FRAMEWORK***";
        }

	};
}