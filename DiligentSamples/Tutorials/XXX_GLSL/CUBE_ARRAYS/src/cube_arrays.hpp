#include "SampleBase.hpp"

namespace Diligent
{
	class CubeArrays final :public SampleBase
	{
    private:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double c, double e) override final;
        virtual const Char* GetSampleName()const override final
        {
            return "CUBE ARRAYS";
        }

    public:
        RefCntAutoPtr<IPipelineState> m_PSO;
        RefCntAutoPtr<IShaderResourceBinding> m_SRB;

        RefCntAutoPtr<IBuffer> VSContants;
        RefCntAutoPtr<IBuffer> VBuffers;
        RefCntAutoPtr<IBuffer> IBuffers;

        void CreatePipeline();
        void CreateUBuffer();
        void LoadBuffers();

        float4x4 MVP;
	};
}