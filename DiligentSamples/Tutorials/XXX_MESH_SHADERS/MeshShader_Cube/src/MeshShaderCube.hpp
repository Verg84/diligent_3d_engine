#include "SampleBase.hpp"

namespace Diligent
{
	class MeshShaderCube final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double ctime, double etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Mesh Shader Cube";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IShaderResourceBinding> m_SRB;
        RefCntAutoPtr<IBuffer> m_CubeBuffer;

        void LoadCubeBuffers();
        void CreatePipelineMS();
	};
}