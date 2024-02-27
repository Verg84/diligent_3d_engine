#include "SampleBase.hpp"

namespace Diligent
{
	class TextureArray final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "XXX::Texture_Arrays";
        }

    private:
        
        void CreatePipeline();
        void CreateUniformBuffer();
        void UpdateInstanceBuffer();

        RefCntAutoPtr<IPipelineState> pipeline_handler;
        
        RefCntAutoPtr<IBuffer> Vertex_Buffer;
        RefCntAutoPtr<IBuffer> Index_Buffer;
        RefCntAutoPtr<IBuffer> Uniform_Buffer;
        RefCntAutoPtr<IBuffer> Instance_Buffer;

        float4x4 ViewProjectionMatrix;
        float4x4 RotationMatrix;

        int GridSize = 5;
        static constexpr int MaxGridSize=32;
        static constexpr int MaxInstances = MaxGridSize*MaxGridSize*MaxGridSize;
        static constexpr int NumTextures  = 4;


	};
}