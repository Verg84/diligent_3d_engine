#include "SampleBase.hpp"

namespace Diligent
{
class UniformBuffers final : public SampleBase
{
public:
    virtual void        Initialize(const SampleInitInfo& InitInfo) override final;
    virtual void        Update(double c, double e) override final;
    virtual void        Render() override final;
    virtual const Char* GetSampleName() const override final
    {
        return "Uniform Buffers";
    }

private:
    RefCntAutoPtr<IPipelineState> m_pPSO;

    RefCntAutoPtr<IBuffer> VBuffers;
    RefCntAutoPtr<IBuffer> IBuffers;
    RefCntAutoPtr<IBuffer> UnifBuffers;

    RefCntAutoPtr<IShaderResourceBinding> m_pSRB;


    void CreatePipeline();
    void CreateVertexBuffers();
    void CreateIndexBuffers();
    void CreateUniformBuffers();

    float4x4 ModelViewProj;
    float    x_coord = 0.0f;
    float    inc     = 0.1f;

};
} // namespace Diligent