#include"SampleBase.hpp"

namespace Diligent
{
	
	enum VERTEX_COMPONENTS_FLAGS:Uint32
	{
		FLAG_NONE=0x00,
		FLAG_POS=0x01,
		FLAG_NORMAL=0x02,
		FLAG_TEXTURE=0x4,
		FLAG_POS_UV=FLAG_POS|FLAG_TEXTURE,
		FLAG_POS_NORM_UV=FLAG_POS|FLAG_TEXTURE|FLAG_NORMAL
	};
	class TexCube final :public SampleBase
	{
		public:
			virtual void Initialize(const SampleInitInfo &InitInfo)override final;
            virtual void Render() override final;
            virtual void Update(double ctime, double etime) override final;
			virtual const Char* GetSampleName()const override final
			{
                return "XXX::Textured Cube";
			}

        private:
            RefCntAutoPtr<IBuffer> CreateVertexBuffer(
                IRenderDevice           *Device,
                VERTEX_COMPONENTS_FLAGS VertexFlags,
                BIND_FLAGS              BindFlags = BIND_VERTEX_BUFFER,
                BUFFER_MODE             Mode      = BUFFER_MODE_UNDEFINED
			);
            RefCntAutoPtr<IBuffer> CreateIndexBuffer(
                IRenderDevice *Device,
                BIND_FLAGS    BindFlags = BIND_INDEX_BUFFER,
                BUFFER_MODE   Mode      = BUFFER_MODE_UNDEFINED
			);
            RefCntAutoPtr<ITexture> LoadTexture(IRenderDevice* Device, const Char* Path);

			struct CreateInfo
			{
                IRenderDevice* Device = nullptr;

				TEXTURE_FORMAT RTVFormat = TEX_FORMAT_UNKNOWN;
                TEXTURE_FORMAT DSVFormat = TEX_FORMAT_UNKNOWN;

				IShaderSourceInputStreamFactory *pShaderSourceFactory=nullptr;
				
				const char* VSPath=nullptr;
                const char* PSPath = nullptr;

				VERTEX_COMPONENTS_FLAGS VertexFlags = FLAG_NONE;

				LayoutElement *ExtraLayoutElems = nullptr;

				Uint32 NumExtraLayoutElements = 0;
                Uint8  SampleCount            = 1;
			};
            RefCntAutoPtr<IPipelineState> CreatePipelineState(const CreateInfo& createInfo);
            void LoadData();
            
			Uint32                        NV = 0;
            Uint32                        NI = 0;

            std::vector<float3>            Positions;
            std::vector<float2>            TexCoords;
            std::vector<float3>            Normals;
            std::vector<Uint32>            Indices;

			RefCntAutoPtr<IPipelineState> pipeline_handler;
            RefCntAutoPtr<IBuffer>        vertex_buffer_handler;
            RefCntAutoPtr<IBuffer>        pixel_buffer_handler;
                
           

	};
}