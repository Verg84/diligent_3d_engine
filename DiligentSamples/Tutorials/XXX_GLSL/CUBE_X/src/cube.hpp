#include"SampleBase.hpp"

namespace Diligent
{
	class CUBE final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double c,double e)override final;
        virtual const Char* GetSampleName() const override final
        {
            return "CUBE ";
        }

    private:
        
        RefCntAutoPtr<IPipelineState> m_PSO;
        
        RefCntAutoPtr<IShaderResourceBinding> m_SRB;

        RefCntAutoPtr<IBuffer> ConstantsBuffer;

        RefCntAutoPtr<IBuffer> VBuf;
        RefCntAutoPtr<IBuffer> IBuf;


        void CreatePipeline();
        void CreateConstantsBuffers();
        void CreateBuffers();

        float4x4 MVP;
	};
}