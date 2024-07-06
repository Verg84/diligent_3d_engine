#include"BaseRenderer.hpp"

Diligent::BaseRenderer::BaseRenderer(
    RefCntAutoPtr<IEngineFactory>& engineFactory,
    RefCntAutoPtr<IRenderDevice>&  device,
    RefCntAutoPtr<ISwapChain>&     swapChain) :
    m_pEngineFactory(engineFactory), m_pDevice(device), m_pSwapChain(swapChain)
{
}
