#pragma once
#include<iostream>
#include<vector>
#include"RefCntAutoPtr.hpp"
#include"Buffer.h"
#include"RenderDevice.h"
#include"DeviceContext.h"
#include <BasicMath.hpp>
#include"BaseScene.hpp"

namespace Diligent
{
	class BaseRenderer
	{
    public:
        virtual void BindScene(BaseScene* scene) = 0;

        virtual void Render(
                            RefCntAutoPtr<IDeviceContext> immediateContext,
                            float4x4                      view,
                            float4x4                      viewproj) = 0;
        
        virtual void Print(std::ostream& out) const = 0;

        virtual void WindowResize();



    protected:
        BaseRenderer(
            RefCntAutoPtr<IEngineFactory> &engineFacory,
            RefCntAutoPtr<IRenderDevice> &device,
            RefCntAutoPtr<ISwapChain> &swapchain);

        RefCntAutoPtr<IPipelineState> m_pPSO;
        RefCntAutoPtr<IEngineFactory> m_pEngineFactory;
        RefCntAutoPtr<IRenderDevice>  m_pDevice;
        RefCntAutoPtr<ISwapChain>     m_pSwapChain;
        RefCntAutoPtr<IBuffer>        m_pConstants;
	};

    inline std::ostream& operator<<(std::ostream& out, const BaseRenderer& renderer)
    {
        renderer.Print(out);
        return out;
    }
    }