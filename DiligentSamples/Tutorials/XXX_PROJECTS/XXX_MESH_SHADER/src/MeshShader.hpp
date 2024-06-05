#include "SampleBase.hpp"

namespace Diligent
{
	class MeshShader final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& IniInfo) override final;
        virtual void Update(double c, double e) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "MESH SHADERS";
        }


        private:
            
            RefCntAutoPtr<IPipelineState> m_pPSO;
            RefCntAutoPtr<IShaderResourceBinding> m_pSRB;
            RefCntAutoPtr<IBuffer> m_CubeBuffer;
            RefCntAutoPtr<ITexture> m_CubeTextureSRV;

            RefCntAutoPtr<IBuffer> m_pStatisticsBuffer;
            RefCntAutoPtr<IBuffer> m_pStatisticsStaging;
            RefCntAutoPtr<IBuffer> m_pStatisticsAvailable;

            Uint64 m_FrameID = 1;
            const Uint32 m_StatisticsHistorySize = 8;

            static constexpr Int32 ASGroupSize=32;

            Uint32 m_DrawTaskCount = 0;
            RefCntAutoPtr<IBuffer> m_pDrawTasks;
            RefCntAutoPtr<IBuffer> m_pConstants;

            float4x4 m_ViewProjMatrix;
            float4x4 m_ViewMatrix;
            float    m_RotationAngle = 0;
            bool     m_Anime         = true;
            bool     m_Frustum_Culling = true;
            const float m_FOV             = PI_F / 4.0f;
            const float m_CoTanHalfFOV    = 1.0f / std::tan(m_FOV * 0.5f);
            float       m_LodScale        = 4.0f;
            float       m_CameraHeight    = 10.0f;
            float       m_CurTime         = 0.0f;
            Uint32      m_VisibleCubes    = 0;
	};
}