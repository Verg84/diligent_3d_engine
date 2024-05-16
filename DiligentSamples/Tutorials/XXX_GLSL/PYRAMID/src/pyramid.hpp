#include"SampleBase.hpp"

namespace Diligent
{
	class Pyramid2d final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Pyramid2D";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_PSO;

        RefCntAutoPtr<IShaderResourceBinding> m_SRB;

        RefCntAutoPtr<ITextureView> m_Texture;
        RefCntAutoPtr<IBuffer>  Vertex_Buffer;
        RefCntAutoPtr<IBuffer>  Index_Buffer;
        RefCntAutoPtr<IBuffer>  m_ConstantsBuffer;

        void CreatePipeline();
        void LoadBuffers();
        void LoadTexture();
        void CreateUniformfBuffer();

        float3 offset;

	};
}