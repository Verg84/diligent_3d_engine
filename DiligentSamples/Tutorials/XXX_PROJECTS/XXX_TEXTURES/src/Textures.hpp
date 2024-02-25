#include"SampleBase.hpp"

namespace Diligent
{
	class Textures final :public SampleBase
	{
		public:
			virtual void Initialize(const SampleInitInfo& InitInfo) override final;
			virtual void Render() override final;
            virtual void Update(double ctime, double etime) override final;
			virtual const Char* GetSampleName()const override final
			{
                return "XXX::Texturing";
			}

        private:
            RefCntAutoPtr<IPipelineState>			pipeline_handler;
            RefCntAutoPtr<IShaderResourceBinding>	resource_binding;
            RefCntAutoPtr<IBuffer>					Vertex_Buffer;
            RefCntAutoPtr<IBuffer>					Index_Buffer;
            RefCntAutoPtr<IBuffer>                  Uniform_Buffer;
            RefCntAutoPtr<ITextureView>             tex_resource;
			
			float4x4 ModelViewProj;


			void CreatePipelineState();
            void CreateBuffers();
            void CreateUniformBuffer();
            void CreateTexture();
	};

}