#include"XXX_TextureArrays.hpp"
#include <random>
#include <string>

#include "MapHelper.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"
#include "ColorConversion.h"
#include "../../Common/src/TexturedCube.hpp"
#include "imgui.h"

namespace Diligent
{
    struct InstanceData
    {
        float4x4 Mat;
        float TexInd=0;
    };
SampleBase* CreateSample()
{
    return new TextureArray();
}
void TextureArray::CreatePipelineState()
{
    LayoutElement elems[] =
        {
            // Buffer Slot 0
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 2, VT_FLOAT32, False},

            // Buffer Slot 1
            LayoutElement{2, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
            LayoutElement{3, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
            LayoutElement{4, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
            LayoutElement{5, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
            // attribute 6 texture array index
            LayoutElement{6, 1, 1, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE}

        };
    RefCntAutoPtr<IShaderSourceInputStreamFactory> spf;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &spf);

    TexturedCube::CreatePSOInfo pso;
    pso.pDevice = m_pDevice;
    pso.RTVFormat = m_pSwapChain->GetDesc().ColorBufferFormat;
    pso.DSVFormat = m_pSwapChain->GetDesc().DepthBufferFormat;
    pso.pShaderSourceFactory = spf;
    pso.VSFilePath           = "inst_texarray_vertex_shaderSrc.vsh";
    pso.PSFilePath           = "inst_texarray_pixel_shaderSrc.psh";
    pso.ExtraLayoutElements  = elems;
    pso.NumExtraLayoutElements = _countof(elems);

    pipeline_handler = TexturedCube::CreatePipelineState(pso);
    CreateUniformBuffer(m_pDevice, sizeof(float4x4) * 2, "Constants", &Constants_Buffer);
    pipeline_handler->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(Constants_Buffer);
    pipeline_handler->CreateShaderResourceBinding(&resource_bind_handler, true);
    
}

void TextureArray::CreateInstanceBuffer()
{
    BufferDesc InstBuffDesc;
    InstBuffDesc.Name      = "Instance Buffer";
    InstBuffDesc.Usage     = USAGE_DEFAULT;
    InstBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
    InstBuffDesc.Size      = sizeof(InstanceData) * MaxInstances;
    m_pDevice->CreateBuffer(InstBuffDesc, nullptr, &Instance_Buffer);

    PopulateInstanceBuffer();
}

void TextureArray::LoadTextures()
{
    std::vector<RefCntAutoPtr<ITextureLoader>> TexLoaders(NumTextures);
    for (int tex = 0; tex < NumTextures; ++tex)
    {
        // current texture loader
        std::stringstream FileNameSS;
        FileNameSS << "DGLogo" << tex << ".png";
        const auto FileName = FileNameSS.str();
        TextureLoadInfo loadInfo;
        loadInfo.IsSRGB=true;

        CreateTextureLoaderFromFile(FileName.c_str(), IMAGE_FILE_FORMAT_UNKNOWN, loadInfo, &TexLoaders[tex]);
        VERIFY_EXPR(TexLoaders[tex]);
        VERIFY(tex == 0 || TexLoaders[tex]->GetTextureDesc() == TexLoaders[0]->GetTextureDesc(), "all textures must have same size");
    }
    auto TexArrayDesc = TexLoaders[0]->GetTextureDesc();
    TexArrayDesc.ArraySize = NumTextures;
    TexArrayDesc.Type      = RESOURCE_DIM_TEX_2D_ARRAY;
    TexArrayDesc.Usage     = USAGE_DEFAULT;
    TexArrayDesc.BindFlags = BIND_SHADER_RESOURCE;

    // Initialization Data
    std::vector<TextureSubResData> SubresData(TexArrayDesc.ArraySize * TexArrayDesc.MipLevels);
    for (Uint32 slice = 0; slice < TexArrayDesc.ArraySize; ++slice)
    {
        for (Uint32 mip = 0; mip < TexArrayDesc.MipLevels; ++mip)
        {
            SubresData[slice * TexArrayDesc.MipLevels + mip] = TexLoaders[slice]->GetSubresourceData(mip, 0);
        }
    }
    TextureData InitData{SubresData.data(), TexArrayDesc.MipLevels * TexArrayDesc.ArraySize};
    // create texture array
    RefCntAutoPtr<ITexture> pTexArray;
    m_pDevice->CreateTexture(TexArrayDesc, &InitData, &pTexArray);

    texture_shader_res = pTexArray->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
    resource_bind_handler->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(texture_shader_res);
}

void TextureArray::PopulateInstanceBuffer()
{
    // Populate instance data buffer
    const auto                zGridSize = static_cast<size_t>(m_GridSize);
    std::vector<InstanceData> InstanceData(zGridSize * zGridSize * zGridSize);

    float fGridSize = static_cast<float>(m_GridSize);

    std::mt19937 gen; // Standard mersenne_twister_engine. Use default seed
                      // to generate consistent distribution.

    std::uniform_real_distribution<float> scale_distr(0.3f, 1.0f);
    std::uniform_real_distribution<float> offset_distr(-0.15f, +0.15f);
    std::uniform_real_distribution<float> rot_distr(-PI_F, +PI_F);
    std::uniform_int_distribution<Int32>  tex_distr(0, NumTextures - 1);

    float BaseScale = 0.6f / fGridSize;
    int   instId    = 0;
    for (int x = 0; x < m_GridSize; ++x)
    {
        for (int y = 0; y < m_GridSize; ++y)
        {
            for (int z = 0; z < m_GridSize; ++z)
            {
                // Add random offset from central position in the grid
                float xOffset = 2.f * (x + 0.5f + offset_distr(gen)) / fGridSize - 1.f;
                float yOffset = 2.f * (y + 0.5f + offset_distr(gen)) / fGridSize - 1.f;
                float zOffset = 2.f * (z + 0.5f + offset_distr(gen)) / fGridSize - 1.f;
                // Random scale
                float scale = BaseScale * scale_distr(gen);
                // Random rotation
                float4x4 rotation = float4x4::RotationX(rot_distr(gen));
                rotation *= float4x4::RotationY(rot_distr(gen));
                rotation *= float4x4::RotationZ(rot_distr(gen));
                // Combine rotation, scale and translation
                float4x4 matrix   = rotation * float4x4::Scale(scale, scale, scale) * float4x4::Translation(xOffset, yOffset, zOffset);
                auto&    CurrInst = InstanceData[instId++];
                CurrInst.Mat   = matrix;
                // Texture array index
                CurrInst.TexInd = static_cast<float>(tex_distr(gen));
            }
        }
    }
    // Update instance data buffer
    Uint32 DataSize = static_cast<Uint32>(sizeof(InstanceData[0]) * InstanceData.size());
    m_pImmediateContext->UpdateBuffer(Instance_Buffer, 0, DataSize, InstanceData.data(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

void TextureArray::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipelineState();
    Vertex_Buffer = TexturedCube::CreateVertexBuffer(m_pDevice,TexturedCube::VERTEX_COMPONENT_FLAG_POS_UV);
    Index_Buffer  = TexturedCube::CreateIndexBuffer(m_pDevice);
    CreateInstanceBuffer();
    LoadTextures();
    
}

void TextureArray::Render()
{
    float ClearColor[] = {0.2f, 0.8f, 0.4f, 1.0f};
    auto* pRTV=m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV         = m_pSwapChain->GetDepthBufferDSV();

    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG,1.0f,0,RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        MapHelper<float4x4> VConstants(m_pImmediateContext, Constants_Buffer, MAP_WRITE, MAP_FLAG_DISCARD);
        VConstants[0] = VProj.Transpose();
        VConstants[1] = Rotation_Mat.Transpose();
    }

    const Uint64 offsets[] = {0, 0};
    IBuffer*     pbuffer[] = {Vertex_Buffer, Instance_Buffer};
    m_pImmediateContext->SetVertexBuffers(0, _countof(pbuffer), pbuffer, offsets, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->SetIndexBuffer(Index_Buffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Set Pipeline State ..... Commit Shader Resources
    m_pImmediateContext->SetPipelineState(pipeline_handler);
    m_pImmediateContext->CommitShaderResources(resource_bind_handler, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Draw commands
    DrawIndexedAttribs drawAttrs;
    drawAttrs.IndexType=VT_UINT32;
    drawAttrs.NumIndices = 36;
    drawAttrs.NumInstances = m_GridSize * m_GridSize * m_GridSize;
    // verify draw state
    drawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
    m_pImmediateContext->DrawIndexed(drawAttrs);

}
void TextureArray::UpdateUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::SliderInt("Grid Size", &m_GridSize, 1, 32))
        {
            PopulateInstanceBuffer();
        }
    }
    ImGui::End();
}
void TextureArray::Update(double ctime, double etime)
{
    SampleBase::Update(ctime, etime);
    UpdateUI();
    // set View Matrix
    float4x4 View            = float4x4::RotationX(-0.6f) * float4x4::Translation(0.0f, 0.0f, 4.0f);
    auto     SrfPerTransform = GetSurfacePretransformMatrix(float3(0, 0, 1));
    auto     Proj            = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);
    VProj                    = View * SrfPerTransform * Proj;
}

}