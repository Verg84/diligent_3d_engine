#include "SampleBase.hpp"

namespace Diligent
{
	class Instancing final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double ctime, double etime) override final;
        virtual const Char* GetSampleName()const override final
        {
            return "XXX::Instancing";
        }

    private:
        RefCntAutoPtr<IPipelineState>           pipeline_handler;
        RefCntAutoPtr<IShaderResourceBinding>   resource_binding;
        RefCntAutoPtr<IBuffer>                  uniform_buffer;
        RefCntAutoPtr<IBuffer>                  vertex_buffer;
        RefCntAutoPtr<IBuffer>                  index_buffer;
        RefCntAutoPtr<IBuffer>                  instance_buffer;
        
        float4x4 Rotation_Matrix;
        float4x4 VP_Matrix;
        
        void CreatePipelineState();
        void CreateBuffers();
        void CreateUniformBuffer();
        void CreateInstanceBuffer();
        void PopulateInstanceBuffer();
        void UpdateUI();

         int                  m_GridSize   = 5;
        static constexpr int MaxGridSize  = 32;
        static constexpr int MaxInstances = MaxGridSize * MaxGridSize * MaxGridSize;
	};
}