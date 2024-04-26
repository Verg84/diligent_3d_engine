#include "SampleBase.hpp"

namespace Diligent
{
    class ASS01 final :public SampleBase
    {
    public:
        virtual void Initialize(const SampleInitInfo& ÎnitInfo) override final;
        virtual void Update(double ctime,double etime)override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName() const override final
        {
            return "ASSIGNMENT 01";
        }

    private:
        RefCntAutoPtr<IPipelineState>           pipeline_handler;
        RefCntAutoPtr<IShaderResourceBinding>   srb;
        RefCntAutoPtr<IBuffer>                  Vertex_Buffer;
        RefCntAutoPtr<IBuffer>                  Index_Buffer;


        void CreatePipelineState();
        void CreateBuffers();
    };
}