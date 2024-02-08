#include"XXX_Instancing.hpp"
namespace Diligent
{
	SampleBase* CreateSample()
	{
        return new Instancing();
    }
    void Instancing::CreateUniformBuffer()
    {
        BufferDesc uniformBufferDesc;
        uniformBufferDesc.Name = "Vertex Shader Constants Buffer";
        uniformBufferDesc.BindFlags = BIND_UNIFORM_BUFFER;
        uniformBufferDesc.Usage=USAGE_DYNAMIC;
        uniformBufferDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        uniformBufferDesc.Size=sizeof(float4x4)*2;
        m_pDevice->CreateBuffer(uniformBufferDesc, nullptr, &uniform_buffer);

    }
	void Instancing::CreatePipelineState()
	{
		// define Layout Elems
        LayoutElement elems[] =
            {
                {0, 0, 3, VT_FLOAT32, False},
                {1, 0, 2, VT_FLOAT32, False},
                {2, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                {3, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                {4, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE},
                {5, 1, 4, VT_FLOAT32, False, INPUT_ELEMENT_FREQUENCY_PER_INSTANCE}
            };
        // shader source factory
        RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
        m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
		// texture setup
		TexturedCube::CreatePSOInfo CubePSOCI;
        CubePSOCI.pDevice = m_pDevice;
        CubePSOCI.RTVFormat = m_pSwapChain->GetDesc().ColorBufferFormat;
        CubePSOCI.DSVFormat = m_pSwapChain->GetDesc().DepthBufferFormat;
        CubePSOCI.pShaderSourceFactory = pShaderSourceFactory;
        CubePSOCI.VSFilePath           = "cube_inst.vsh";
        CubePSOCI.PSFilePath           = "cube_inst.psh";
        CubePSOCI.ExtraLayoutElements  = elems;
        CubePSOCI.NumExtraLayoutElements = _countof(elems);

        pipeline_handler = TexturedCube::CreatePipelineState(CubePSOCI);
        CreateUniformBuffer();
        pipeline_handler->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(uniform_buffer);

        pipeline_handler->CreateShaderResourceBinding(&resource_binding, true);

	}
	void Instancing::Initialize(const SampleInitInfo& InitInfo)
	{
        SampleBase::Initialize(InitInfo);
        CreatePipelineState();
	}
	void Instancing::Update(double ctime, double etime)
	{
        SampleBase::Update(ctime, etime);
	}
	void Instancing::Render()
	{

	}
}