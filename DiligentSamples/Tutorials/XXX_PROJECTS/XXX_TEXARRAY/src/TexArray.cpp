#include "TexArray.hpp"
#include <random>
#include <string>
#include "MapHelper.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"
#include "ColorConversion.h"
#include "imgui.h"

namespace Diligent
{
namespace
{

struct InstanceData
{
    float4x4 Matrix;
    float    TextureInd = 0;
};

} // namespace
	SampleBase* CreateSample()
	{
        return new TextureArray();
    }
    void TextureArray::UpdateInstanceBuffer()
    {
        const auto zGridSize = static_cast<size_t>(GridSize);
        std::vector<InstanceData> InstanceData(zGridSize * zGridSize * zGridSize);
        float fGridSize=static_cast<float>(GridSize);

        std::mt19937 gen;

        std::uniform_real_distribution<float> scale_distr(0.3f, 1.0f);
        std::uniform_real_distribution<float> offset_distr(-0.15f, 0.15f);
        std::uniform_real_distribution<float> rot_distr(-PI_F, PI_F);
        std::uniform_real_distribution<float> tex_distr(0, NumTextures - 1);

        float BaseScale = 0.6f / fGridSize;
        int   instanceID = 0;
        for (int x = 0; x < GridSize; ++x)
        {
            for (int y = 0; y < GridSize; ++y)
            {
                for (int z = 0; z < zGridSize; ++z)
                {
                    float xOffset = 2.f * (x + 0.5f + offset_distr(gen)) / fGridSize-1.f;
                    float yOffset = 2.f * (y + 0.5f + offset_distr(gen)) / fGridSize - 1.f;
                    float zOffset = 2.f * (z + 0.5f + offset_distr(gen)) / fGridSize - 1.f;

                    // RADOM SCALE
                    float scale = BaseScale * scale_distr(gen);
                   // RANDOM ROTATION
                    float4x4 rotation = float4x4::RotationX(rot_distr(gen));
                    rotation *= float4x4::RotationY(rot_distr(gen));
                    rotation *= float4x4::RotationZ(rot_distr(gen));

                    float4x4 matrix = rotation * float4x4::Scale(scale, scale, scale) * float4x4::Translation(xOffset, yOffset, zOffset);
                    auto&    CurrInst = InstanceData[instanceID++];
                    CurrInst.Matrix   = matrix;

                    CurrInst.TextureInd = static_cast<float>(tex_distr(gen));
                }
            }
        }
        // UPDATE INSTANCE BUFFER
        Uint32 DataSize = static_cast<Uint32>(sizeof(InstanceData[0]) * InstanceData.size());
        m_pImmediateContext->UpdateBuffer(Instance_Buffer, 0, DataSize, InstanceData.data(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    }
    void TextureArray::CreateUniformBuffer()
    {
        BufferDesc uDesc;
        uDesc.Name = "Vertex Uniform Variables";
        uDesc.BindFlags = BIND_UNIFORM_BUFFER;
        uDesc.Usage     = USAGE_DYNAMIC;
        uDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        uDesc.Size           = sizeof(float4x4) * 2;
        m_pDevice->CreateBuffer(uDesc, nullptr, &Uniform_Buffer);
    }
	void TextureArray::CreatePipeline()
	{
        GraphicsPipelineStateCreateInfo pso;
        pso.PSODesc.Name = "INSTANCING GRAPHICS PIPELINE";
        pso.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
        pso.GraphicsPipeline.NumRenderTargets = 1;
        pso.GraphicsPipeline.RTVFormats[0]    = m_pSwapChain->GetDesc().ColorBufferFormat;
        pso.GraphicsPipeline.DSVFormat        = m_pSwapChain->GetDesc().DepthBufferFormat;
        pso.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		pso.GraphicsPipeline.RasterizerDesc.CullMode=CULL_MODE_BACK;
        pso.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

		ShaderCreateInfo sci;
        sci.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
        sci.Desc.UseCombinedTextureSamplers = true;

		RefCntAutoPtr<IShaderSourceInputStreamFactory> sf;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &sf);
        sci.pShaderSourceStreamFactory = sf;

		RefCntAutoPtr<IShader> vertex_shader;
		{
            sci.Desc.Name = "VERTEX SHADER";
            sci.Desc.ShaderType = SHADER_TYPE_VERTEX;
            sci.EntryPoint      = "main";
            sci.FilePath        = "texarray_inst.vsh";
            m_pDevice->CreateShader(sci, &vertex_shader);
		}
        RefCntAutoPtr<IShader> pixel_shader;
        {
            sci.Desc.Name = "PIXEL SHADER";
            sci.Desc.ShaderType = SHADER_TYPE_PIXEL;
            sci.EntryPoint      = "main";
            sci.FilePath        = "texarray_inst.psh";
            m_pDevice->CreateShader(sci,&pixel_shader);
        }
        pso.pVS = vertex_shader;
        pso.pPS = pixel_shader;

        CreateUniformBuffer();

        LayoutElement layoutelems[] =
            {
                // Buffer Slot No.0 ---> attrib0 , attrib1
                LayoutElement{0, 0, 3, VT_FLOAT32, False},
                LayoutElement{1, 0, 2, VT_FLOAT32, False},
                // Buffer Slot No.1 ---> attrib2,....,atrib6
                LayoutElement{2, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{3, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{4, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                LayoutElement{5, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},

                LayoutElement{6, 1, 1, VT_FLOAT32, False, LAYOUT_ELEMENT_AUTO_OFFSET,
                                                          LAYOUT_ELEMENT_AUTO_STRIDE, 
                                                          INPUT_ELEMENT_FREQUENCY_PER_INSTANCE}

            };
        pso.GraphicsPipeline.InputLayout.LayoutElements = layoutelems;
        pso.GraphicsPipeline.InputLayout.NumElements    = _countof(layoutelems);

        pso.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;


	}

	void TextureArray::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        CreatePipeline();
	}
	void TextureArray::Render()
	{

	}
	void TextureArray::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
	}
}