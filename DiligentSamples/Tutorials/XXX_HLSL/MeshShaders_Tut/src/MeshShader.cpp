#include"MeshShader.hpp"
#include "FastRand.hpp"
#include "AdvancedMath.hpp"

namespace Diligent
{
namespace
{
    #include"../assets/structures.fxh"
}
SampleBase* CreateSample()
{
    return new MeshShader();
}
void MeshShader::CreateDrawTasks() 
{
    const int2 GridDim{128, 128};
    FastRandReal<float> Rnd{0, 0.0f, 1.0f};
    std::vector<DrawTask> DrawTasks;
    DrawTasks.resize(static_cast<size_t>(GridDim.x) * static_cast<size_t>(GridDim.y));

    for (int y = 0; y < GridDim.y; ++y)
    {
        for (int x = 0; x < GridDim.x; ++x)
        {
            int idx = x + y * GridDim.x;
            auto& dst = DrawTasks[idx];
            dst.BasePos.x = (x - GridDim.x / 2) * 4.0f + (Rnd() * 2.0f - 1.0f);
            dst.BasePos.y = (x - GridDim.y / 2) * 4.0f + (Rnd() * 2.0f - 1.0f);
            dst.Scale     = Rnd() * 0.5f + 0.5f;
        }
    }

    BufferDesc bufDesc;
    bufDesc.Name = "draw tasks buffer";
    bufDesc.Usage = USAGE_DEFAULT;
    bufDesc.BindFlags = BIND_SHADER_RESOURCE;
    bufDesc.Mode      = BUFFER_MODE_STRUCTURED;
    bufDesc.ElementByteStride = sizeof(DrawTasks[0]);
    bufDesc.Size              = sizeof(DrawTasks[0]) * static_cast<Uint32>(DrawTasks.size());

    BufferData bufData;
    bufData.pData = DrawTasks.data();
    bufData.DataSize = bufDesc.Size;

    m_pDevice->CreateBuffer(bufDesc, &bufData, &m_DrawTasksBuffer);
    VERIFY_EXPR(m_DrawTasksBuffer != nullptr);
}
void MeshShader::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreateDrawTasks();
}
void MeshShader::Update(double ctime,double etime)
{
    SampleBase::Update(ctime, etime);
}
void MeshShader::Render()
{

}
}