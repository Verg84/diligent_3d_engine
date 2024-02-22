#include "SampleBase.hpp"

namespace Diligent
{
	class Triangle final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo)override final;
        virtual void Render() override final;
        virtual void Update(double ctime, double etime) override final;
        virtual const Char* GetSampleName() const override final
        {
            return "XXX::TRIANGLE";
        }

    private:
        RefCntAutoPtr<IPipelineState> pipeline_handler;
        RefCntAutoPtr<IBuffer>         vertex_buffer;

        void CreatePipelineState();
        void CreateVertexBuffer();
	};
}

