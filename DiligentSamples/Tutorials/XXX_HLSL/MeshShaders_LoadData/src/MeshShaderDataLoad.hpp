#include"SampleBase.hpp"

namespace Diligent
{
	class MeshShaderDataLoad final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Mesh Shader Data Load";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IBuffer> m_ObjectBuffer;
        RefCntAutoPtr<IShaderResourceBinding> m_SRB;

        void CreatePipelineMeshShader();
        void CreateObject();
        
	};
}