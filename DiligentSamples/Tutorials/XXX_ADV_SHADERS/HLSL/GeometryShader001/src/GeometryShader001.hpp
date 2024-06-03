#include "SampleBase.hpp"
namespace Diligent
{

	class GeomShader_PassThrough final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double ctime, double etime) override final;
        virtual const Char* GetSampleName()const override final
        {
            return "Geometry Shader Pass Through";
        }

    private:
        
        RefCntAutoPtr<IPipelineState> m_PSO;
        RefCntAutoPtr<IShaderResourceBinding> m_SRB;
        RefCntAutoPtr<IBuffer>  VertexBuffer;

        void LoadBuffers();
        void CreatePipelineState();

	};



    }