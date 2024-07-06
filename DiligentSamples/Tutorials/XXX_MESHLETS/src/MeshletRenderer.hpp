#pragma once
#include"BaseRenderer.hpp"

namespace Diligent
{
	class MeshletRenderer final :BaseRenderer
	{
    public:
        MeshletRenderer(
            RefCntAutoPtr<IEngineFactory>& engineFactory,
            RefCntAutoPtr<IRenderDevice>&  device,
            RefCntAutoPtr<ISwapChain>&     swapChain);

        void BindScene(BaseScene* scene);
        void Render(RefCntAutoPtr<IDeviceContext> immediateContext, float4x4 view, float4x4 viewProj);
        void Print(std::ostream& out)const;
    private:
		struct Constants
		{
            float4x4 ViewMat;
            float4x4 ViewProjMat;

			float4 ViewUp;
            float4 ViewDirection;
            float4 Frustum[6];

			Uint32 MeshletCount;
            Uint32 MeshletCountPerInstance;
            Uint32 MeshCountPerInstance;
		};

		float4x4 lastViewMatrix;
        float4x4 lastViewProjMatrix;
        bool     isViewFrozen = false;

		Uint32 m_pMeshletCount = 0;

		void CreatePipelineState();

	};
}
