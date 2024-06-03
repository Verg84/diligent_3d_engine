#include "SampleBase.hpp"

namespace Diligent
{
	class GeomShader final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double ctime, double etime) override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Geometry Shader";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;

        RefCntAutoPtr<IBuffer> VertexBuffer;
        RefCntAutoPtr<IBuffer> IndexBuffer;
        RefCntAutoPtr<IBuffer> ShaderConstants;

        RefCntAutoPtr<IShaderResourceBinding> m_SRB;

        void CreatePipelineState();
        void CreateUniformBuffer();
        void LoadCubeBuffers();

         float4x4 m_WorldViewProjMatrix;
         float    m_LineWidth = 2.0f;
	};
}