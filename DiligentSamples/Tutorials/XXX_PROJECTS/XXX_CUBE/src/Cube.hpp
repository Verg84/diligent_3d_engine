#include"SampleBase.hpp"

namespace Diligent
{
	class Cube final :public SampleBase
	{
    public:
		
		virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double ctime, double etime) override final;
		virtual const Char* GetSampleName()const override final
		{
            return "XXX::Cube";
		}

    private:
        RefCntAutoPtr<IPipelineState>		    pipeline_handler;
        RefCntAutoPtr<IBuffer>			    	Vertex_Buffer;
        RefCntAutoPtr<IBuffer>                  Index_Buffer;
        RefCntAutoPtr<IBuffer>                  Uniform_Buffer;
        RefCntAutoPtr<IShaderResourceBinding>   resource_binding;
        float4x4                                ModelViewProj;

		void CreatePipelineState();
        void CreateBuffers();
        void CreateUniformBuffer();
	};
}