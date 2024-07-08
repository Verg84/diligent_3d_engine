#include"SampleBase.hpp"

namespace Diligent
{
	class MeshShaderTriangle final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Triangle Mesh Shader";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IShaderResourceBinding> m_SRB;
        RefCntAutoPtr<IBuffer> m_TriangleBuffer;

        void CreatePipelineState();
        void LoadTriangle();

        struct TriangleData
        {
            float4 Positions[3];
            uint4  Indices[1];
        };
	};
}