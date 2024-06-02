#include"SampleBase.hpp"

namespace Diligent
{
	class TexturesX final : public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double c, double e) override final;
        virtual const Char* GetSampleName() const override final
        {
            return "Texture X";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;

        void CreatePipeline();
	};
}