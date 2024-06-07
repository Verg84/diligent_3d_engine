#pragma once

#include "SampleBase.hpp"
#include "BasicMath.hpp"

namespace Diligent
{

class MeshShader final : public SampleBase
{
public:
    virtual void ModifyEngineInitInfo(const ModifyEngineInitInfoAttribs& Attribs) override final;
    virtual void Initialize(const SampleInitInfo& InitInfo) override final;

    virtual void Render() override final;
    virtual void Update(double CurrTime, double ElapsedTime) override final;

    virtual const Char* GetSampleName() const override final { return "Tutorial20: Mesh shader"; }

private:
    void CreatePipelineState();
    void CreateCube();
    void CreateDrawTasks();
    void CreateStatisticsBuffer();
    void CreateConstantsBuffer();
    void LoadTexture();
    void UpdateUI();

    RefCntAutoPtr<IBuffer>      m_CubeBuffer;
    RefCntAutoPtr<ITextureView> m_CubeTextureSRV;

    RefCntAutoPtr<IBuffer> m_pStatisticsBuffer;
    RefCntAutoPtr<IBuffer> m_pStatisticsStaging;
    RefCntAutoPtr<IFence>  m_pStatisticsAvailable;
    Uint64                 m_FrameId               = 1; // Can't signal 0
    const Uint32           m_StatisticsHistorySize = 8;

    static constexpr Int32 ASGroupSize = 32;

    Uint32                 m_DrawTaskCount = 0;
    RefCntAutoPtr<IBuffer> m_pDrawTasks;
    RefCntAutoPtr<IBuffer> m_pConstants;

    RefCntAutoPtr<IPipelineState>         m_pPSO;
    RefCntAutoPtr<IShaderResourceBinding> m_pSRB;

    float4x4    m_ViewProjMatrix;
    float4x4    m_ViewMatrix;
    float       m_RotationAngle  = 0;
    bool        m_Animate        = true;
    bool        m_FrustumCulling = true;
    const float m_FOV            = PI_F / 4.0f;
    const float m_CoTanHalfFov   = 1.0f / std::tan(m_FOV * 0.5f);
    float       m_LodScale       = 4.0f;
    float       m_CameraHeight   = 10.0f;
    float       m_CurrTime       = 0.0f;
    Uint32      m_VisibleCubes   = 0;
};

} // namespace Diligent