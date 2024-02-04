#include"SampleBase.hpp"

namespace Diligent
{
class Instancing final : public SampleBase
{
public:
    virtual void        Initialize(const SampleInitInfo& InitInfo) override final;
    virtual void        Render() override final;
    virtual void        Update(double ctime, double etime) override final;
    virtual const Char* GetSampleName() const override final
    {
        return "XXX::INSTANCING";
    }

private:
    RefCntAutoPtr<IPipelineState>         pipeline_handler;
    RefCntAutoPtr<IShader>                vertex_shader_handler;
    RefCntAutoPtr<IShader>                pixel_shader_handler;
    RefCntAutoPtr<IShaderResourceBinding> resource_binding;
    RefCntAutoPtr<ITextureView>           texture_handler;
    RefCntAutoPtr<IBuffer>                uniform_buffer;
    RefCntAutoPtr<IBuffer>                vertex_buffer;
    RefCntAutoPtr<IBuffer>                index_buffer;

    void PipelineStateInit(const Char* vertex_shader_src, const Char* pixel_shader_src);
    void CreatePipelineParams(GraphicsPipelineStateCreateInfo& psi);
    void LoadShaders(const Char* vertex_shader_src, const Char* pixel_shader_src, GraphicsPipelineStateCreateInfo& psi);
    void Loadtexture();
    void CreateUniformBuffer();
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreatePipelineState();
};


struct Vertex
 {
    float3 Position;
    float2 uV;
};
 Vertex CubeVerts[] =
     {
         {float3(-1, -1, -1), float2(0, 1)},
         {float3(-1, +1, -1), float2(0, 0)},
         {float3(+1, +1, -1), float2(1, 0)},
         {float3(+1, -1, -1), float2(1, 1)},

         {float3(-1, -1, -1), float2(0, 1)},
         {float3(-1, -1, +1), float2(0, 0)},
         {float3(+1, -1, +1), float2(1, 0)},
         {float3(+1, -1, -1), float2(1, 1)},

         {float3(+1, -1, -1), float2(0, 1)},
         {float3(+1, -1, +1), float2(1, 1)},
         {float3(+1, +1, +1), float2(1, 0)},
         {float3(+1, +1, -1), float2(0, 0)},

         {float3(+1, +1, -1), float2(0, 1)},
         {float3(+1, +1, +1), float2(0, 0)},
         {float3(-1, +1, +1), float2(1, 0)},
         {float3(-1, +1, -1), float2(1, 1)},

         {float3(-1, +1, -1), float2(1, 0)},
         {float3(-1, +1, +1), float2(0, 0)},
         {float3(-1, -1, +1), float2(0, 1)},
         {float3(-1, -1, -1), float2(1, 1)},

         {float3(-1, -1, +1), float2(1, 1)},
         {float3(+1, -1, +1), float2(0, 1)},
         {float3(+1, +1, +1), float2(0, 0)},
         {float3(-1, +1, +1), float2(1, 0)}};

Uint32 Indices[] =
    {
        2, 0, 1, 2, 3, 0,
        4, 6, 5, 4, 7, 6,
        8, 10, 9, 8, 11, 10,
        12, 14, 13, 12, 15, 14,
        16, 18, 17, 16, 19, 18,
        20, 21, 22, 20, 22, 23};
    

} // namespace Diligent