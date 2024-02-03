/*
 *  Copyright 2019-2022 Diligent Graphics LLC
 *  Copyright 2015-2019 Egor Yusov
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#include <random>

#include "Tutorial04_Instancing.hpp"
#include "MapHelper.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"
#include "../../Common/src/TexturedCube.hpp"
#include "imgui.h"

namespace Diligent
{

SampleBase* CreateSample()
{
    return new Tutorial04_Instancing();
}

void Tutorial04_Instancing::CreatePipelineState()
{
    // 1. Initialize the PSO
    GraphicsPipelineStateCreateInfo PSOInst;
    PSOInst.PSODesc.Name         = "Instancing Pipeline State - PSO";
    PSOInst.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
    // 2. Set Render Target and formats
    PSOInst.GraphicsPipeline.NumRenderTargets = 1;
    PSOInst.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOInst.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
    // 3. States Options
    PSOInst.GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    PSOInst.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;
    PSOInst.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
    // 4. Shaders set up
    ShaderCreateInfo ShaderCI;
    ShaderCI.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_HLSL;
    ShaderCI.Desc.UseCombinedTextureSamplers = true;
    // 5. shader Factory
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderS;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderS);
    ShaderCI.pShaderSourceStreamFactory = pShaderS;
    // 6. vertex shader
    RefCntAutoPtr<IShader> vs;
    {
        ShaderCI.Desc.Name       = "vertex shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "cube_inst.vsh";
        m_pDevice->CreateShader(ShaderCI, &vs);
    }
    // 7. pixel shader
    RefCntAutoPtr<IShader> ps;
    {
        ShaderCI.Desc.Name       = "pixel shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "cube_inst.psh";
        m_pDevice->CreateShader(ShaderCI, &ps);
    }
    // 8. input layout
    LayoutElement layout[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, false},
            LayoutElement{1, 0, 2, VT_FLOAT32, false},

            LayoutElement{2, 1, 4, VT_FLOAT32, false, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
            LayoutElement{3, 1, 4, VT_FLOAT32, false, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
            LayoutElement{4, 1, 4, VT_FLOAT32, false, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
            LayoutElement{5, 1, 4, VT_FLOAT32, false, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
        };
    PSOInst.GraphicsPipeline.InputLayout.LayoutElements = layout;
    PSOInst.GraphicsPipeline.InputLayout.NumElements    = _countof(layout);
    PSOInst.pVS                                         = vs;
    PSOInst.pPS                                         = ps;
    // 9. Shader Resources
    ShaderResourceVariableDesc Vars[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}};
    SamplerDesc samplerLinearClamp{
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};
    ImmutableSamplerDesc imSampler[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", samplerLinearClamp}};
    PSOInst.PSODesc.ResourceLayout.DefaultVariableType  = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    PSOInst.PSODesc.ResourceLayout.Variables            = Vars;
    PSOInst.PSODesc.ResourceLayout.NumVariables         = _countof(Vars);
    PSOInst.PSODesc.ResourceLayout.ImmutableSamplers    = imSampler;
    PSOInst.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(imSampler);
    // 10. Create the Pipeline State
    m_pDevice->CreateGraphicsPipelineState(PSOInst, &m_pPSO);
    // 11. Uniform buffer
    BufferDesc vConstantsDesc;
    vConstantsDesc.Name           = "Vertex Constants buffer";
    vConstantsDesc.Usage          = USAGE_DYNAMIC;
    vConstantsDesc.BindFlags      = BIND_UNIFORM_BUFFER;
    vConstantsDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
    vConstantsDesc.Size           = sizeof(float4x4) * 2;
    m_pDevice->CreateBuffer(vConstantsDesc, nullptr, &m_VSConstants);
    // 12. Set static shader resource variables into the pipeline
    m_pPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_VSConstants);
    // 13. Create SRB Object for mutable variables
    m_pPSO->CreateShaderResourceBinding(&m_SRB, true);
}

void Tutorial04_Instancing::CreateVertexBuffer()
{
    struct Vertex
    {
        float3 pos;
        float2 uv;
    };
    constexpr Vertex CubeVerts[] =
        {
            {float3{-1, -1, -1}, float2{0, 1}},
            {float3{-1, +1, -1}, float2{0, 0}},
            {float3{+1, +1, -1}, float2{1, 0}},
            {float3{+1, -1, -1}, float2{1, 1}},

            {float3{-1, -1, -1}, float2{0, 1}},
            {float3{-1, -1, +1}, float2{0, 0}},
            {float3{+1, -1, +1}, float2{1, 0}},
            {float3{+1, -1, -1}, float2{1, 1}},

            {float3{+1, -1, -1}, float2{0, 1}},
            {float3{+1, -1, +1}, float2{1, 1}},
            {float3{+1, +1, +1}, float2{1, 0}},
            {float3{+1, +1, -1}, float2{0, 0}},

            {float3{+1, +1, -1}, float2{0, 1}},
            {float3{+1, +1, +1}, float2{0, 0}},
            {float3{-1, +1, +1}, float2{1, 0}},
            {float3{-1, +1, -1}, float2{1, 1}},

            {float3{-1, +1, -1}, float2{1, 0}},
            {float3{-1, +1, +1}, float2{0, 0}},
            {float3{-1, -1, +1}, float2{0, 1}},
            {float3{-1, -1, -1}, float2{1, 1}},

            {float3{-1, -1, +1}, float2{1, 1}},
            {float3{+1, -1, +1}, float2{0, 1}},
            {float3{+1, +1, +1}, float2{0, 0}},
            {float3{-1, +1, +1}, float2{1, 0}},
        };
    BufferDesc VertBufDesc;
    VertBufDesc.Name      = "Vertex Buffer";
    VertBufDesc.Usage     = USAGE_IMMUTABLE;
    VertBufDesc.BindFlags = BIND_VERTEX_BUFFER;
    VertBufDesc.Size      = sizeof(CubeVerts);
    BufferData VertBufData;
    VertBufData.pData    = CubeVerts;
    VertBufData.DataSize = sizeof(CubeVerts);
    m_pDevice->CreateBuffer(VertBufDesc, &VertBufData, &m_CubeVertexBuffer);
}

void Tutorial04_Instancing::CreateIndexBuffer()
{
    Uint32 Indices[] =
        {
            2, 0, 1, 2, 3, 0,
            4, 6, 5, 4, 7, 6,
            8, 10, 9, 8, 11, 10,
            12, 14, 13, 12, 15, 14,
            16, 18, 17, 16, 19, 18,
            20, 21, 22, 20, 22, 23};

    BufferDesc IBufDesc;
    IBufDesc.Name = "Vertex Indices";
    IBufDesc.Usage = USAGE_IMMUTABLE;
    IBufDesc.BindFlags = BIND_INDEX_BUFFER;
    IBufDesc.Size      = sizeof(Indices);
    BufferData IBufData;
    IBufData.pData = Indices;
    IBufData.DataSize = sizeof(Indices);
    m_pDevice->CreateBuffer(IBufDesc, &IBufData, &m_CubeIndexBuffer);

    
}

void Tutorial04_Instancing::LoadTexture()
{
    TextureLoadInfo texLoadInfo;
    texLoadInfo.IsSRGB = true;
    RefCntAutoPtr<ITexture> Tex;
    CreateTextureFromFile("DGLogo.png", texLoadInfo, m_pDevice, &Tex);
    m_TextureSRV = Tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);
    m_SRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(m_TextureSRV);

}

void Tutorial04_Instancing::CreateInstanceBuffer()
{
    BufferDesc InstBufDesc;
    InstBufDesc.Name = "Instances buffer";
    InstBufDesc.BindFlags = BIND_VERTEX_BUFFER;
    InstBufDesc.Usage = USAGE_DEFAULT;
    InstBufDesc.Size  = sizeof(float4x4) * MaxInstances;
    m_pDevice->CreateBuffer(InstBufDesc, nullptr,&m_InstanceBuffer);
    PopulateInstanceBuffer();
}

void Tutorial04_Instancing::UpdateUI()
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

void Tutorial04_Instancing::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipelineState();
    CreateInstanceBuffer();
    CreateVertexBuffer();
    CreateIndexBuffer();
    LoadTexture();


}

void Tutorial04_Instancing::PopulateInstanceBuffer()
{
    const auto zGridSize = static_cast<size_t>(m_GridSize);
    std::vector<float4x4> InstanceData(zGridSize * zGridSize * zGridSize);
    
    float fGridSize = static_cast<float>(m_GridSize);
    std::mt19937 gen;

    std::uniform_real_distribution<float> scale_distr(0.3f, 1.0f);
    std::uniform_real_distribution<float> offset_distr(-0.15f, 0.15f);
    std::uniform_real_distribution<float> rot_distr(-PI_F, PI_F);

    float BaseScae = 0.6f / fGridSize;
    int   instId    = 0;
    for (int x = 0; x < m_GridSize; ++x)
    {
        for (int y = 0; y < m_GridSize; ++y)
        {
            for (int z = 0; z < m_GridSize; ++z)
            {
                // random offset
                float xOffset = 2.0f * (x + 0.5f + offset_distr(gen)) / fGridSize - 1.0f;
                float yOffset = 2.0f * (y + 0.5f + offset_distr(gen)) / fGridSize - 1.0f;
                float zOffset = 2.0f * (z + 0.5f + offset_distr(gen)) / fGridSize - 1.0f;
                // random scale
                float scale = BaseScae * scale_distr(gen);
                // Random rotation
                float4x4 rotation = float4x4::RotationX(rot_distr(gen));
                rotation *= float4x4::RotationY(rot_distr(gen));
                rotation *= float4x4::RotationZ(rot_distr(gen));
                // combine all
                float4x4 matrix = rotation * float4x4::Scale(scale, scale, scale) * float4x4::Translation(xOffset, yOffset, zOffset);
                InstanceData[instId++] = matrix;
            }
        }
    }
    Uint32 DataSize = static_cast<Uint32>(sizeof(InstanceData[0])*InstanceData.size());
    m_pImmediateContext->UpdateBuffer(m_InstanceBuffer,0,DataSize,InstanceData.data(),RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}


// Render a frame
void Tutorial04_Instancing::Render()
{
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
    const float ClearColor[]= {1.0f, 0.0f, 0.0f, 1.0f};

    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        MapHelper<float4x4> Const(m_pImmediateContext, m_VSConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        Const[0] = m_ViewProjMatrix.Transpose();
        Const[1] = m_RotationMatrix.Transpose();
    }

    const Uint64 offsets[] = {0, 0};
    IBuffer*     pBuffer[]   = {m_CubeVertexBuffer, m_InstanceBuffer};
    m_pImmediateContext->SetVertexBuffers(0, _countof(pBuffer), pBuffer, offsets, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    m_pImmediateContext->SetIndexBuffer(m_CubeIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    m_pImmediateContext->SetPipelineState(m_pPSO);
    m_pImmediateContext->CommitShaderResources(m_SRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    DrawIndexedAttribs dAtrr;
    dAtrr.IndexType = VT_UINT32;
    dAtrr.NumIndices = 36;
    dAtrr.NumInstances = m_GridSize * m_GridSize * m_GridSize;
    dAtrr.Flags        = DRAW_FLAG_VERIFY_ALL;
    m_pImmediateContext->DrawIndexed(dAtrr);
}

void Tutorial04_Instancing::Update(double CurrTime, double ElapsedTime)
{
    SampleBase::Update(CurrTime, ElapsedTime);
    UpdateUI();

    
    // Set cube view matrix
    float4x4 View = float4x4::RotationX(-0.6f) * float4x4::Translation(0.f, 0.f, 4.0f);

    // Get pretransform matrix that rotates the scene according the surface orientation
    auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

    // Get projection matrix adjusted to the current screen orientation
    auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

    // Compute view-projection matrix
    m_ViewProjMatrix = View * SrfPreTransform * Proj;

    // Global rotation matrix
    m_RotationMatrix = float4x4::RotationY(static_cast<float>(CurrTime) * 1.0f) * float4x4::RotationX(-static_cast<float>(CurrTime) * 0.25f);

   
}

} // namespace Diligent
