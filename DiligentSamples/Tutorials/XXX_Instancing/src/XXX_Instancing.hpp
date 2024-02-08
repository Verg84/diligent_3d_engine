#pragma once
#include "SampleBase.hpp"
#include "../../Common/src/TexturedCube.hpp"

namespace Diligent
{
class Instancing final : public SampleBase
{
public:
    virtual void Initialize(const SampleInitInfo& InitInfo) override final;
    virtual void Render() override final;
    virtual void Update(double ctime, double etime)override final;
    virtual const Char* GetSampleName() const override final
    {
        return "XXX::Instancing";
    }

private:
    RefCntAutoPtr<IPipelineState> pipeline_handler;
    RefCntAutoPtr<IBuffer> uniform_buffer;
    RefCntAutoPtr<IShaderResourceBinding> resource_binding;
    float4x4 RotationMat;
    float4x4 ViewProjMat;
    int mGridSize=5;
    static constexpr int MaxGridSize=32;
    static constexpr int MaxInstances=MaxGridSize*MaxGridSize*MaxGridSize;
    void CreatePipelineState();   
    void CreateUniformBuffer();
    void CreateInstanceBuffer();
};
}