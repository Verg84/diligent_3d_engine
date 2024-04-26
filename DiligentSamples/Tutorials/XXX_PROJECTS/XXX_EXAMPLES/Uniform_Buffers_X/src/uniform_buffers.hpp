#include"SampleBase.hpp"

namespace Diligent
{
	class UniformBuffers final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double c, double e) override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Uniform Buffers";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IShaderResourceBinding> m_pSRB;
        RefCntAutoPtr<IBuffer> Vertex_Buffer;
        RefCntAutoPtr<IBuffer> Index_Buffer;
        RefCntAutoPtr<IBuffer> Uniforms_Buffer;

        float4x4 MVP;
        
        void CreatePipelineState();
        void CreateBuffers();
	};
}