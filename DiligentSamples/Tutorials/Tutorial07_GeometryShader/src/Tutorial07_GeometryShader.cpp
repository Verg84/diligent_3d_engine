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

#include "Tutorial07_GeometryShader.hpp"
#include "MapHelper.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"
#include "../../Common/src/TexturedCube.hpp"
#include "imgui.h"

namespace Diligent
{

SampleBase* CreateSample()
{
    return new Tutorial07_GeometryShader();
}

namespace
{

struct Constants
{
    float4x4 WorldViewProj;
    float4   ViewportSize;
    float    LineWidth;
};

} // namespace

void Tutorial07_GeometryShader::CreatePipelineState()
{
    GraphicsPipelineStateCreateInfo PSO_GS;
    PSO_GS.PSODesc.Name = "Geometry Shader Pipeline";
    PSO_GS.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    // Render Target - RTV Format - DSV Format
    PSO_GS.GraphicsPipeline.NumRenderTargets = 1;
    PSO_GS.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
    PSO_GS.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;

    // Primitive Topology - Rasterizer setup - Depth Stencil buffer setup
    PSO_GS.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    PSO_GS.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    PSO_GS.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

    // SHADERS 
    ShaderCreateInfo ShaderCI;
    ShaderCI.SourceLanguage                  = SHADER_SOURCE_LANGUAGE_HLSL;
    ShaderCI.Desc.UseCombinedTextureSamplers = true;
    // Shader Source Stream Factory
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr,&pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;

    // Vertex Shader
    RefCntAutoPtr<IShader> v_shader;
    {
        ShaderCI.Desc.Name = "Vertex Shader GS";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "cube.vsh";
        m_pDevice->CreateShader(ShaderCI, &v_shader);
        // Uniform Buffer for Constants;
        BufferDesc ConstDesc;
        ConstDesc.Name = "Shader Constants Buffer";
        ConstDesc.Usage = USAGE_DYNAMIC;
        ConstDesc.BindFlags = BIND_UNIFORM_BUFFER;
        ConstDesc.Size           = sizeof(Constants);
        ConstDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        m_pDevice->CreateBuffer(ConstDesc, nullptr,&m_ShaderConstants);
    }
    // Geometry Shader
    RefCntAutoPtr<IShader> g_shader;
    {
        ShaderCI.Desc.Name = "Geometry Shader GS";
        ShaderCI.Desc.ShaderType = SHADER_TYPE_GEOMETRY;
        ShaderCI.FilePath        = "cube.gsh";
        ShaderCI.EntryPoint      = "main";
        m_pDevice->CreateShader(ShaderCI, &g_shader);
    }
    // Pixel Shader
    RefCntAutoPtr<IShader> p_shader;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.Desc.Name       = "Shader Pixel GS";
        ShaderCI.EntryPoint      = "main";
        ShaderCI.FilePath        = "cube.psh";
        m_pDevice->CreateShader(ShaderCI, &p_shader);
    }

    // Input Layout
    LayoutElement layout[] =
        {
            LayoutElement{0, 0, 3, VT_FLOAT32, False},
            LayoutElement{1, 0, 2, VT_FLOAT32, False}};
    PSO_GS.GraphicsPipeline.InputLayout.LayoutElements = layout;
    PSO_GS.GraphicsPipeline.InputLayout.NumElements = _countof(layout);

    PSO_GS.pVS = v_shader;
    PSO_GS.pGS = g_shader;
    PSO_GS.pPS = p_shader;

    // Shader Resource Variables
    PSO_GS.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;
    // texture resource - mutable shader resource variable
    ShaderResourceVariableDesc Vars[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}
        };
    PSO_GS.PSODesc.ResourceLayout.Variables = Vars;
    PSO_GS.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    SamplerDesc SamLinearClampDesc{
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP};
    // texture sampler - immutable shader resource variable
    ImmutableSamplerDesc im[] =
        {
            {SHADER_TYPE_PIXEL, "g_Texture", SamLinearClampDesc}};
    PSO_GS.PSODesc.ResourceLayout.ImmutableSamplers = im;
    PSO_GS.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(im);

    m_pDevice->CreateGraphicsPipelineState(PSO_GS, &m_pPSO);


}

void Tutorial07_GeometryShader::UpdateUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SliderFloat("Line Width", &m_LineWidth, 1.f, 10.f);
    }
    ImGui::End();
}

void Tutorial07_GeometryShader::ModifyEngineInitInfo(const ModifyEngineInitInfoAttribs& Attribs)
{
    SampleBase::ModifyEngineInitInfo(Attribs);

    Attribs.EngineCI.Features.GeometryShaders = DEVICE_FEATURE_STATE_ENABLED;
}


void Tutorial07_GeometryShader::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);
    CreatePipelineState();

    
}

// Render a frame
void Tutorial07_GeometryShader::Render()
{
   
}

void Tutorial07_GeometryShader::Update(double CurrTime, double ElapsedTime)
{
    SampleBase::Update(CurrTime, ElapsedTime);
    UpdateUI();

    // Apply rotation
    float4x4 CubeModelTransform = float4x4::RotationY(static_cast<float>(CurrTime) * 1.0f) * float4x4::RotationX(-PI_F * 0.1f);

    // Camera is at (0, 0, -5) looking along the Z axis
    float4x4 View = float4x4::Translation(0.f, 0.0f, 5.0f);

    // Get pretransform matrix that rotates the scene according the surface orientation
    auto SrfPreTransform = GetSurfacePretransformMatrix(float3{0, 0, 1});

    // Get projection matrix adjusted to the current screen orientation
    auto Proj = GetAdjustedProjectionMatrix(PI_F / 4.0f, 0.1f, 100.f);

    // Compute world-view-projection matrix
    m_WorldViewProjMatrix = CubeModelTransform * View * SrfPreTransform * Proj;
}

} // namespace Diligent
