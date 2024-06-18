#include "SampleBase.hpp"

namespace Diligent
{
class MeshShaderSolids final : public SampleBase
{
public:
    virtual void        Initialize(const SampleInitInfo& InitInfo) override final;
    virtual void        Update(double ctime, double etime) override final;
    virtual void        Render() override final;
    virtual const Char* GetSampleName() const override final
    {
        return "Mesh Shaders - Solid Trinalges";
    }

private:
    RefCntAutoPtr<IPipelineState> m_pPSO;

    void CreatePipelineMeshShader();
};
} // namespace Diligent