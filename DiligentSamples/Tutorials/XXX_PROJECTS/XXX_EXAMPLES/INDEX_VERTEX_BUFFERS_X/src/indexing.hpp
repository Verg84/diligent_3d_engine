#include"SampleBase.hpp"

namespace Diligent
{
	class Indexing final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double e, double c) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName() const override final
        {
            return "INDEXING";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IBuffer> vertex_buffer;
        RefCntAutoPtr<IBuffer> index_buffer;
        RefCntAutoPtr<IShaderResourceBinding> m_pSRB;

        void CreatePipeline();
        void CreateBuffers();

	};
}