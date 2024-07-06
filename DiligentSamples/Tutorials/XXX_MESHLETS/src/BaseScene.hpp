#pragma once
#include<iostream>
#include"RefCntAutoPtr.hpp"
#include"Buffer.h"
#include"RenderDevice.h"
#include"BasicMath.hpp"

namespace Diligent
{
	class BaseRenderer;
	class BaseScene
	{
    public:
        
        RefCntAutoPtr<IBuffer> m_pMeshletInfos;
        RefCntAutoPtr<IBuffer> m_pMeshlets;
        RefCntAutoPtr<IBuffer> m_pIndices;
        RefCntAutoPtr<IBuffer> m_pVertexIndices;
        RefCntAutoPtr<IBuffer> m_pVertices;
        RefCntAutoPtr<IBuffer> m_pMeshInfos;
        RefCntAutoPtr<IBuffer> m_pGlobalIndices;

        Uint32 m_pMeshletCount = 0;
        Uint32 m_pMeshletCountPerInstance = 0;
        Uint32 m_pMeshCountPerInstance    = 0;
        Uint32 m_InstanceCount            = 0;
        Uint32 m_IndicesPerInstance       = 0;
        Uint32 m_MeshInfoCount            = 0;

        virtual void Print(std::ostream& out) const = 0;
        virtual void WindowResize(Uint32 width, Uint32 height) = 0;
        virtual void GetSceneCamera(float4x4* view, float4x4* viewProjection) = 0;

    protected:

        RefCntAutoPtr<IRenderDevice> m_pDevice;
        BaseScene(RefCntAutoPtr<IRenderDevice> device);

        void ReleaseAllBuffers();
        void CreateBuffer(
            const char*             name,
            size_t                  size,
            size_t                  stride,
            const void*             data,
            RefCntAutoPtr<IBuffer>& buffer,
            BIND_FLAGS              bindFlags);
	};
}
