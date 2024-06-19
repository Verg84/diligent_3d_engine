#include"SampleBase.hpp"
#include"MapHelper.hpp"

namespace Diligent
{

	class MeshShader final :public SampleBase
	{
    public:
        
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName() const override final
        {
            return "Mesh Shader Tutotial";
        }

    private:
        
        void CreateMeshPipeline();
        void CreateConstantsBuffer();
        void CreateCubeObject();
        void CreateDrawTasks();
        
        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IShaderResourceBinding> m_pSRB;
        RefCntAutoPtr<IBuffer>                m_CubeBuffer;
        RefCntAutoPtr<IBuffer>                m_ConstantsBuffer;
        RefCntAutoPtr<IBuffer>                m_DrawTasksBuffer;

        static constexpr Int32 ASGroupSize = 32;
        Uint32                 m_pDrwTasksCount = 0;

        float4x4 m_ViewProjMatrix;
        float4x4 m_Viewmatrix;

        float m_RotationAngle = 0;
        bool  m_FrustumCulling = true;
        const float m_FOV            = PI_F / 4.0f;
        const float m_CoTanHalfFov   = 1.0f / std::tan(m_FOV * 0.5f);
        float       m_LodScale       = 4.0f;
        float       m_CameraHeight   = 10.0f;
        float       m_CurrTime       = 0.0f;
        Uint32      m_VisibleCubes   = 0;



	};
}
