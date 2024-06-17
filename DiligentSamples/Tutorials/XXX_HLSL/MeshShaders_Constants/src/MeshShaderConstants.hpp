#include"SampleBase.hpp"

namespace Diligent
{
	class MeshShaderConstants final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Mesh Shader Constants";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IShaderResourceBinding> m_SRB;
        RefCntAutoPtr<IBuffer> m_ConstantsBuffer;
        RefCntAutoPtr<IBuffer> m_ObjectBuffer;

        void CreatePipeMeshShader();
        void CreateObject();
        void CreateConstantsBuffer();

        float4x4 g_ViewMat;
        float4x4 g_ViewProjMat;
        const float m_FOV = PI_F / 4.0f;
	};
}