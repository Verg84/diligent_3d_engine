#include"SampleBase.hpp"

namespace Diligent
{
	class Instancing final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo)override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "INSTANCING HLSL";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;

        RefCntAutoPtr<IBuffer> m_Constants;

        RefCntAutoPtr<IBuffer> m_Vertex_Buffer;
        RefCntAutoPtr<IBuffer> m_Index_Buffer;

        RefCntAutoPtr<IBuffer> m_Instance;

        RefCntAutoPtr<IShaderResourceBinding> m_SRB;

        void CreatePipeline();
        void CreateInstanceBuffer();
        void PopulateInstanceBuffer();
        void UpdateUI();
        void CreateBuffers();
        void CreateUniformBuffer();

        float4x4             m_ViewProjMatrix;
        float4x4             m_RotationMatrix;
        int                  m_GridSize   = 5;
        static constexpr int MaxGridSize  = 32;
        static constexpr int MaxInstances = MaxGridSize * MaxGridSize * MaxGridSize;
	};
}