#include"SampleBase.hpp"

namespace Diligent
{
	class MObject final :public SampleBase
	{
    public:

        virtual void Initialize(const SampleInitInfo& InitInfo)override final;
        virtual void Update(double ctime, double etime)override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "MObject Project";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;

        RefCntAutoPtr<IBuffer> m_VertexBuffer;
        RefCntAutoPtr<IBuffer> m_IndexBuffer;
        
        void CreatePipeline();
	};

}