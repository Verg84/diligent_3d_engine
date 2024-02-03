#include"SampleBase.hpp"
#include"MapHelper.hpp"
namespace Diligent
{
	class Functions final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "XXX::Funstions";
        }

    private:
        void PipelineStateInit();
        void LoadShaders(const Char *vsrc,const Char *psrc);
        void CreateUniformBuffer();
        void LoadVertices();
        void LoadIndices();
        void Clear();
        void SetUniformMatrix();
        void BindBuffers();
        void Draw();
        void LoadTexture();

        RefCntAutoPtr<IPipelineState>         pipeline_handler;
        RefCntAutoPtr<IShader>                vertex_shader;
        RefCntAutoPtr<IShader>                pixel_shader;
        RefCntAutoPtr<IBuffer>                uniform_buffer;
        RefCntAutoPtr<IBuffer>                positions_buffer;
        RefCntAutoPtr<IBuffer>                index_buffer;
        RefCntAutoPtr<IShaderResourceBinding> resouce_handler;
        RefCntAutoPtr<ITextureView>           texture_handler;
        float4x4                              MVP;

        
	};
}