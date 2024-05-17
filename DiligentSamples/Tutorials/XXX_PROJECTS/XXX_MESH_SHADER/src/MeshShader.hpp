#include "SampleBase.hpp"

namespace Diligent
{
	class MeshShader final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& IniInfo) override final;
        virtual void Update(double c, double e) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName()
        {
            return "Mesh Shader Project";
        }


        private:


	};
}