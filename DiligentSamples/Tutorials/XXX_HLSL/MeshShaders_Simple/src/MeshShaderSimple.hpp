#include"SampleBase.hpp"

namespace Diligent
{
	class MeshShaderSimple final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Mesh Shader Simple";
        }

    public:
        RefCntAutoPtr<IPipelineState> m_pPSO;

        void CreateMeshShaderPipeline();
	};
}