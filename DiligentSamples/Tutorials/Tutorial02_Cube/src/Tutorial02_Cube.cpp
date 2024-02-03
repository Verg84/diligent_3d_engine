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

#include "Tutorial02_Cube.hpp"
#include "MapHelper.hpp"
#include "GraphicsUtilities.h"

namespace Diligent
{

SampleBase* CreateSample()
{
    return new Tutorial02_Cube();
}

void Tutorial02_Cube::CreatePipelineState()
{
    // PSO Initialization
    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    PSOCreateInfo.PSODesc.Name = "Cube PSO";
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
    
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSOCreateInfo.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
    
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

    // SHADERS
    ShaderCreateInfo ShaderCI;
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
    ShaderCI.Desc.UseCombinedTextureSamplers = true;
    
    // ShaderSourceFactory
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;

    // Vertex Shader
    RefCntAutoPtr<IShader> vertex_shader;
    {
        ShaderCI.Desc.Name = "cube vertex shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "cube.vsh";
        m_pDevice->CreateShader(ShaderCI, &vertex_shader);
        // Shader resources - Uniform Buffer
        BufferDesc ConstBuffer;
        ConstBuffer.Name = "cube transformation matrix";
        ConstBuffer.Usage = USAGE_DYNAMIC;
        ConstBuffer.BindFlags = BIND_UNIFORM_BUFFER;
        ConstBuffer.CPUAccessFlags = CPU_ACCESS_WRITE;
        ConstBuffer.Size           = sizeof(float4x4);
        m_pDevice->CreateBuffer(ConstBuffer, nullptr,&m_VSConstants);
    }
    // Pixel Shader
    RefCntAutoPtr<IShader> pixel_shader;
    {
        ShaderCI.Desc.Name = "cube pixel shader";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "cube.psh";
        m_pDevice->CreateShader(ShaderCI, &pixel_shader);
    }

    // Input Layout
    LayoutElement LayoutElems[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 4, VT_FLOAT32, False}};
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(LayoutElems);

    PSOCreateInfo.pVS = vertex_shader;
    PSOCreateInfo.pPS = pixel_shader;

    // Deault Variable
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

    // Bound static resource to the buffer
    m_pPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_VSConstants);

    // Shader resource binding object
    m_pPSO->CreateShaderResourceBinding(&m_pSRB, true);
}

void Tutorial02_Cube::CreateVertexBuffer()
{
    // Layout of this structure matches the one we defined in the pipeline state
    struct Vertex
    {
        float3 pos;
        float4 color;
    };

    // Cube vertices

    //      (-1,+1,+1)________________(+1,+1,+1)
    //               /|              /|
    //              / |             / |
    //             /  |            /  |
    //            /   |           /   |
    //(-1,-1,+1) /____|__________/(+1,-1,+1)
    //           |    |__________|____|
    //           |   /(-1,+1,-1) |    /(+1,+1,-1)
    //           |  /            |   /
    //           | /             |  /
    //           |/              | /
    //           /_______________|/
    //        (-1,-1,-1)       (+1,-1,-1)
    //

    // clang-format off
    Vertex CubeVerts[8] =
    {
        {float3(-1,-1,-1), float4(1,0,0,1)},
        {float3(-1,+1,-1), float4(0,1,0,1)},
        {float3(+1,+1,-1), float4(0,0,1,1)},
        {float3(+1,-1,-1), float4(1,1,1,1)},

        {float3(-1,-1,+1), float4(1,1,0,1)},
        {float3(-1,+1,+1), float4(0,1,1,1)},
        {float3(+1,+1,+1), float4(1,0,1,1)},
        {float3(+1,-1,+1), float4(0.2f,0.2f,0.2f,1)},
    };
    // clang-format on

    BufferDesc VertexBufferDesc;
    VertexBufferDesc.Name = "vertex positions";
    VertexBufferDesc.Usage = USAGE_IMMUTABLE;
    VertexBufferDesc.BindFlags = BIND_VERTEX_BUFFER;
    VertexBufferDesc.Size      = sizeof(CubeVerts);
    BufferData VData;
    VData.pData = CubeVerts;
    VData.DataSize = sizeof(CubeVerts);
    m_pDevice->CreateBuffer(VertexBufferDesc, &VData, &m_CubeVertexBuffer);
   
}

void Tutorial02_Cube::CreateIndexBuffer()
{
    // clang-format off
    Uint32 Indices[] =
    {
        2,0,1, 2,3,0,
        4,6,5, 4,7,6,
        0,7,4, 0,3,7,
        1,0,4, 1,4,5,
        1,5,2, 5,6,2,
        3,6,7, 3,2,6
    };
    // clang-format on
    BufferDesc IndexBufferDesc;
    IndexBufferDesc.Name = "vertex cube indices";
    IndexBufferDesc.Usage = USAGE_IMMUTABLE;
    IndexBufferDesc.BindFlags = BIND_INDEX_BUFFER;
    IndexBufferDesc.Size      = sizeof(Indices);
    BufferData IndexData;
    IndexData.pData = Indices;
    IndexData.DataSize = sizeof(Indices);
    m_pDevice->CreateBuffer(IndexBufferDesc, &IndexData,&m_CubeIndexBuffer);

}

void Tutorial02_Cube::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipelineState();
    CreateVertexBuffer();
    CreateIndexBuffer();
   
}

// Render a frame
void Tutorial02_Cube::Render()
{
    
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
    // Clear the back buffer
    const float ClearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};
    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        // Map the buffer and write current world-view-projection matrix
        MapHelper<float4x4> CBConstants(m_pImmediateContext, m_VSConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        *CBConstants = m_WorldViewProjMatrix.Transpose();
    }

    // Bind vertex and index buffers
    const Uint64 offset   = 0;
    IBuffer*     pBuffs[] = {m_CubeVertexBuffer};
    m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    m_pImmediateContext->SetIndexBuffer(m_CubeIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Set the pipeline state
    m_pImmediateContext->SetPipelineState(m_pPSO);
    // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
    // makes sure that resources are transitioned to required states.
    m_pImmediateContext->CommitShaderResources(m_pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
    DrawAttrs.IndexType  = VT_UINT32; // Index type
    DrawAttrs.NumIndices = 36;
    // Verify the state of vertex and index buffers
    DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
    m_pImmediateContext->DrawIndexed(DrawAttrs);

   
}

void Tutorial02_Cube::Update(double CurrTime, double ElapsedTime)
{
    // Apply rotation
    float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(CurrTime) * 1.0f) * float4x4::RotationX(-PI_F * 0.1f);

    // Camera is at (0, 0, -5) looking along the Z axis
    float4x4 View = float4x4::Translation(0.f, 0.0f, 8.0f);

    // Get pretransform matrix that rotates the scene according the surface orientation
    auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

    // Get projection matrix adjusted to the current screen orientation
    auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

    // Compute world-view-projection matrix
    m_WorldViewProjMatrix = CubeModelTransform * View * SrfPreTransform * Proj;
}

} // namespace Diligent
