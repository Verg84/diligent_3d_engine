#pragma once
#include "SampleBase.hpp"

namespace Diligent
{
class XXX_Cube final : public SampleBase
{
public:
    virtual void        Initialize(const SampleInitInfo& InitInfo) override final;
    virtual void        Update(double CurTime, double ElTine) override final;
    virtual void        Render() override final;
    virtual const Char* GetSampleName()const override final
    {
        return "XXX Cube";
    }

private:
    void CreatePipelineState();
    void CreateUniformBuffer();
    void CreateVertexBuffers();
    void CreateIndexBuffers();
    void LoadShaders(const Char* vSrc, const Char* pSrc);

    RefCntAutoPtr<IPipelineState>       pipeline_state;
    
    RefCntAutoPtr<IShader>              vertex_shader;
    RefCntAutoPtr<IShader>              pixel_shader;

    RefCntAutoPtr<IBuffer>              constants_buffer;
    float4x4                            MVP;
    RefCntAutoPtr<IBuffer>              vertex_buffer;
    RefCntAutoPtr<IBuffer>              index_bufer;

    RefCntAutoPtr<IShaderResourceBinding> resource_binding;
};
} // namespace Diligent