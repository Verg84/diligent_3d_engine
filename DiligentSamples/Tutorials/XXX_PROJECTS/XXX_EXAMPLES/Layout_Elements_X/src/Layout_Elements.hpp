#include "SampleBase.hpp"

namespace Diligent
{
	class Layout_Elements final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double Ctime, double Etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Layout Elements";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IShaderResourceBinding> m_pSRB;
        RefCntAutoPtr<IBuffer>                vertex_buffer;

        void CreateVertexBuffer();
	};
}
