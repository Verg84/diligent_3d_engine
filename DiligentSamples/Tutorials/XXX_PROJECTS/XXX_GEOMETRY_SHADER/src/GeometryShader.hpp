#include"SampleBase.hpp"

namespace Diligent
{
	class GeometryShader final :public SampleBase
	{
    public:

        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Update(double Ctime, double Etime) override final;
        virtual void Render() override final;
        virtual const Char* GetSampleName() const override final
        {
            return "XXX::GEOMETRY SHADER";
        }

    private:


	};
}