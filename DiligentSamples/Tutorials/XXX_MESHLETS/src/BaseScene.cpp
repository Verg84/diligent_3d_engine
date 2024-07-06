#include"BaseScene.hpp"

Diligent::BaseScene::BaseScene(RefCntAutoPtr<IRenderDevice> device) :
    m_pDevice(device) 
{
}

void Diligent::BaseScene::ReleaseAllBuffers()
{
    m_pMeshlets.Release();
    m_pMeshletInfos.Release();
    m_pIndices.Release();
    m_pVertexIndices.Release();
    m_pVertices.Release();
    m_pMeshInfos.Release();
    m_pGlobalIndices.Release();
}

void Diligent::BaseScene::CreateBuffer(
    const char* name,
    size_t size,
    size_t stride,
    const void* data,
    RefCntAutoPtr<IBuffer>& buffer,
    BIND_FLAGS bindFlags)
{
    buffer.Release();

    BufferDesc BuffDesc;
    BuffDesc.Name = name;
    BuffDesc.Usage = USAGE_DEFAULT;
    BuffDesc.BindFlags = bindFlags;
    BuffDesc.Mode      = BUFFER_MODE_STRUCTURED;
    BuffDesc.ElementByteStride = stride;
    BuffDesc.Size=size;

    BufferData BufData;
    BufData.pData = data;
    BufData.DataSize = BuffDesc.Size;

    m_pDevice->CreateBuffer(BuffDesc, &BufData, &buffer);
    VERIFY_EXPR(buffer != nullptr);

}
