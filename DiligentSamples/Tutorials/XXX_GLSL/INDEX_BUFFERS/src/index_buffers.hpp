#include "SampleBase.hpp"

namespace Diligent
{
	class IndexBuffers final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double c, double e) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Index Buffers";
        }
        private:
        RefCntAutoPtr<IPipelineState> m_pPSO;

        RefCntAutoPtr<IBuffer> VBuffers;
        RefCntAutoPtr<IBuffer> IBuffers;


        void CreatePipeline();
        void CreateVertexBuffers();
        void CreateIndexBuffers();
	};
}