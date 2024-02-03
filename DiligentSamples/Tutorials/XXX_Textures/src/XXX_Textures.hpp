#include "SampleBase.hpp"
namespace Diligent
{
	class Texturing final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double Ctime, double Etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "XXX::TEXTURING";
        }

    private:
        void CreatePipeState();
        void CreateUniformBuffer();
        void LoadShaders(const Char* vsrc,const Char* psrc);
        void CreateVertexBuffer();
        void CreateIndexBuffer();
        void LoadTexture();
        RefCntAutoPtr<IPipelineState> pipe_handler;
        RefCntAutoPtr<IBuffer>        uniform_buf_handler;
        RefCntAutoPtr<IShader>        vertex_shader;
        RefCntAutoPtr<IShader>        pixel_shader;
        RefCntAutoPtr<IShaderResourceBinding> resource_handler;
        RefCntAutoPtr<IBuffer>            vertex_buf_handler;
        RefCntAutoPtr<IBuffer>            index_buf_handler;
        RefCntAutoPtr<ITextureView>           texture_handler;
        float4x4                              MVP;
	};
}