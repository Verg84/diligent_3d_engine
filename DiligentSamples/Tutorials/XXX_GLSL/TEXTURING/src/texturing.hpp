#include "SampleBase.hpp"

namespace Diligent
{
	class Texturing final : public SampleBase
	{
		public:
			virtual void Initialize(const SampleInitInfo& InitInfo) override final;
            virtual void Render() override final;
            virtual void Update(double c, double e) override final;
            virtual const Char* GetSampleName()const override final
            {
                return "Texturing";
			}

        private:
            RefCntAutoPtr<IPipelineState> m_pPSO;
            RefCntAutoPtr<IShaderResourceBinding> m_SRB;

            RefCntAutoPtr<IBuffer> VSConstants;
            RefCntAutoPtr<IBuffer> VSBuffer;
            RefCntAutoPtr<IBuffer> IndexBuffer;

            RefCntAutoPtr<ITextureView> m_TextureSRV;
            


            void CreatePipeline();
            void LoadConstantsUniformBuffer();
            void LoadCubeBuffers();
            void LoadTexture();

            float4x4 MVP;
	};
}