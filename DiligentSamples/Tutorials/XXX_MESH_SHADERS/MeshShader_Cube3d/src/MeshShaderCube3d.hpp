#include"SampleBase.hpp"

namespace Diligent
{
	class MeshShaderCube3d final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Mesh Shader Cube 3d";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IShaderResourceBinding> m_SRB;
        RefCntAutoPtr<IBuffer> m_ObjectBuffer;
        RefCntAutoPtr<IBuffer> m_ConstantsBuffer;

        void CreatePipeline();
        void LoadObjectData();
        void CreateConstantsBuffer();

        float4x4 g_ViewMat;
        float4x4 g_ProjViewMat;
        const float m_FOV = PI_F / 4.0f;

        struct ObjectData
        {
            float4 Positions[24];
            uint4  Indices[36 / 3];
        };
	};
}


