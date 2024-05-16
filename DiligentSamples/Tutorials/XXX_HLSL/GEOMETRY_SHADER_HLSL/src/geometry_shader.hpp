#include "SampleBase.hpp"

namespace Diligent
{
class GeomShader final : public SampleBase
{
public:
    virtual void        Initialize(const SampleInitInfo& InitInfo) override final;
    virtual void        Update(double c, double e) override final;
    virtual void        Render() override final;
    virtual const Char* GetSampleName()
    {
        return "Geometry Shader";
    }
};
} // namespace Diligent