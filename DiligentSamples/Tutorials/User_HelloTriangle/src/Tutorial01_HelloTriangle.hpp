#pragma once

#include"SampleBase.hpp"

namespace Diligent
{
	class User_HelloTriangle final :public SampleBase
	{
    public:
        virtual void Initialize(const SampleInitInfo& InitInfo) override final;
        virtual void Render() override final;
        virtual void Update(double CurTime, double ElapsedTime) override final;
        virtual const Char* GetSampleName() const override final 
        {
            return "User_Hello_Triangle";
        }

    private:
        RefCntAutoPtr<IPipelineState> m_pPSO;
	};
}