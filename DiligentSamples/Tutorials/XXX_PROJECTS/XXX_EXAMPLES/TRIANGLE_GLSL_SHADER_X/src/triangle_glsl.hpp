#include "SampleBase.hpp"

namespace Diligent
{
    
	class Triangle_glsl final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double currentTime, double elapsedTime) override final;
        virtual const Char* GetSampleName() const override final
        {
            return "TRIANGLE_GLSL";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;

        
	};
}