#include"SampleBase.hpp"
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
        RefCntAutoPtr<IPipelineState> pipeline_handler;
        RefCntAutoPtr<IShader>        vertex_shader;
        RefCntAutoPtr<IShader>        pixel_shader;

	};
}