#include"SampleBase.hpp"
namespace Diligent
{
	class TextureArray final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double ctime, double etime) override final;
        virtual const Char* GetSampleName()const override final
        {
            return "XXX::Texture::Arrays";
        }

    private:
        void CreatePipelineState();
        void CreateInstanceBuffer();
        void LoadTextures();
        void UpdateUI();
        void PopulateInstanceBuffer();

        RefCntAutoPtr<IPipelineState> pipeline_handler;

        RefCntAutoPtr<IBuffer> Vertex_Buffer;
        RefCntAutoPtr<IBuffer> Index_Buffer;
        RefCntAutoPtr<IBuffer> Instance_Buffer;
        RefCntAutoPtr<IBuffer> Constants_Buffer;

        RefCntAutoPtr<ITextureView> texture_shader_res;

        RefCntAutoPtr<IShaderResourceBinding> resource_bind_handler;

        float4x4 VProj;
        float4x4 Rotation_Mat;

        int m_GridSize = 5;
        static constexpr int MaxGridSize = 32;
        static constexpr int MaxInstances = MaxGridSize * MaxGridSize * MaxGridSize;
        static constexpr int NumTextures  = 4;
	};
}