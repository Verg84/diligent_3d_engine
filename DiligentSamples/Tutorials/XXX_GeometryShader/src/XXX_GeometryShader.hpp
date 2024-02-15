#include"SampleBase.hpp"
#include"BasicMath.hpp"

namespace Diligent
{
	class GeometryShader final :public SampleBase
	{
        public:
        virtual void Initialize(const SampleInitInfo& InitInfo)override final;
        virtual void Render() override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void ModifyEngineInitInfo(const ModifyEngineInitInfoAttribs& Attribs) override final;
        virtual const Char* GetSampleName()
        {
            return "Geometry::Shader";
        }

    private:
        void CreatePipelineState();
        void UpdateUI();
        void CreateUniformBuffer();

        RefCntAutoPtr<IPipelineState> pipeline_handler;

        RefCntAutoPtr<IBuffer> Vertex_Buffer;
        RefCntAutoPtr<IBuffer> Index_Buffer;
        RefCntAutoPtr<IBuffer> Shader_Constants;

        RefCntAutoPtr<ITextureView> texture_res;
        RefCntAutoPtr<IShaderResourceBinding> resource_binding;

        float4x4 WorldVProj;
        float    line_width = 3.f;

	};
}