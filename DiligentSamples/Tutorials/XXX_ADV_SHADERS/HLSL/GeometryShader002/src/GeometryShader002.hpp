#include "SampleBase.hpp"

namespace Diligent
{
	class GeomShader002 final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Geometry Shader 002";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_PSO;
        RefCntAutoPtr<IShaderResourceBinding> m_SRB;
        RefCntAutoPtr<IBuffer> Vertex_Buffer;

        void CreatePipelineState();
        void LoadBuffers();
	};
}