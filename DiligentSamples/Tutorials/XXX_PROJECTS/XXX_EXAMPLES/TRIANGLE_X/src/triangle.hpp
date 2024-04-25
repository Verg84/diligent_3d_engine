#include"SampleBase.hpp"

namespace Diligent
{
	class Triangle final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double currentTime, double elapsedTime) override final;
        virtual const Char* GetSampleName() const override final
        {
            return "TRIANGLE X";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;
	};
}