#include"SampleBase.hpp"

namespace Diligent
{
	class MSCurves final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Mesh Shader Curves";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;

        void CreateMeshPipeline();
	};
}