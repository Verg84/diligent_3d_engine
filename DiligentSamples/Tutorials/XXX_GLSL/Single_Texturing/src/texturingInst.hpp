#include "SampleBase.hpp"

namespace Diligent
{
	class TextureInst final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo)override final;
        virtual void Update(double etime, double ctime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()
        {
            return "Texturing Single Instance";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_PSO;
        RefCntAutoPtr<IShaderResourceBinding> m_SRB;

        RefCntAutoPtr<IBuffer> m_Constants;
        RefCntAutoPtr<IBuffer> Vertex_Buffer;
        RefCntAutoPtr<IBuffer> Index_Buffer;

        RefCntAutoPtr<ITextureView> m_texture;

        void CreateUniformBuffer();
        void CreateBuffers();
        void CreatePipeline();
        void LoadTexture();

        float4x4 MVP;

	};
}