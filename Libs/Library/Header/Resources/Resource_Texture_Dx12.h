#pragma once
#include"stdafx.h"
#include"Resource_Texture.h"
#include"Header/Device/ImageFileIO.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;
	class Resource_Texture_Dx12 :public Resource_Texture
	{
		struct CUSTAM_SUBRESOURCE_DATA
		{
			const void* pSysMem;
			std::uint32_t SysMemPitch;
			std::uint32_t SysMemSlicePitch;
		};
		enum  TEX_DIMENSION
		{
			TEX_DIMENSION_TEXTURE1D = 2,
			TEX_DIMENSION_TEXTURE2D = 3,
			TEX_DIMENSION_TEXTURE3D = 4,
		}; 
		enum TEX_MISC_FLAG
		// Subset here matches D3D10_RESOURCE_MISC_FLAG and D3D11_RESOURCE_MISC_FLAG
		{
			TEX_MISC_TEXTURECUBE = 0x4L,
		};

		enum TEX_MISC_FLAG2
		{
			TEX_MISC2_ALPHA_MODE_MASK = 0x7L,
		};

		enum TEX_ALPHA_MODE
			// Matches DDS_ALPHA_MODE, encoded in MISC_FLAGS2
		{
			TEX_ALPHA_MODE_UNKNOWN = 0,
			TEX_ALPHA_MODE_STRAIGHT = 1,
			TEX_ALPHA_MODE_PREMULTIPLIED = 2,
			TEX_ALPHA_MODE_OPAQUE = 3,
			TEX_ALPHA_MODE_CUSTOM = 4,
		};
	public:
		struct TexMetadata
		{
			size_t          width;
			size_t          height;     // Should be 1 for 1D textures
			size_t          depth;      // Should be 1 for 1D or 2D textures
			size_t          arraySize;  // For cubemap, this is a multiple of 6
			size_t          mipLevels;
			uint32_t        miscFlags;
			uint32_t        miscFlags2;
			DXGI_FORMAT     format;
			TEX_DIMENSION   dimension;

			size_t  ComputeIndex(_In_ size_t mip, _In_ size_t item, _In_ size_t slice) const;
			// Returns size_t(-1) to indicate an out-of-range error

			bool IsCubemap() const { return (miscFlags & TEX_MISC_TEXTURECUBE) != 0; }
			// Helper for miscFlags

			bool  IsPMAlpha() const { return ((miscFlags2 & TEX_MISC2_ALPHA_MODE_MASK) == TEX_ALPHA_MODE_PREMULTIPLIED) != 0; }
			void  SetAlphaMode(TEX_ALPHA_MODE mode) { miscFlags2 = (miscFlags2 & ~static_cast<uint32_t>(TEX_MISC2_ALPHA_MODE_MASK)) | static_cast<uint32_t>(mode); }
			TEX_ALPHA_MODE GetAlphaMode() const { return static_cast<TEX_ALPHA_MODE>(miscFlags2 & TEX_MISC2_ALPHA_MODE_MASK); }
			// Helpers for miscFlags2

			bool IsVolumemap() const { return (dimension == TEX_DIMENSION_TEXTURE3D); }
			// Helper for dimension
		};

		struct Image
		{
			Value_ptr<ImageFileIO::TextureResourceData> vlp_imageData;
			void* p_resourceData = nullptr;
			DXGI_FORMAT format;
		};
		Resource_Texture_Dx12(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, Value_ptr<GraphicDevice> arg_graphicDevice);
		~Resource_Texture_Dx12() {
		}
		void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;
		void Attach(std::int32_t slot)override;
		void CreateTexture(Image* srcImages, size_t nimages, const TexMetadata& metadata);
		void ToPNG(const std::string& arg_textureFilePath);
		GUIWindowReaction ShowTextureWindow(const std::string& arg_windowName, const std::int32_t winFrag)override;
		void ShowGUI()override;
		void* GetResourcePtr()override;
		virtual void CreateTextureUploadHeap();
		void Initialize()override;
		Microsoft::WRL::ComPtr<ID3D12Resource> GetTexture();
		Microsoft::WRL::ComPtr<ID3D12Resource> GetTextureUploadHeap();
		D3D12_RESOURCE_DESC GetTextureDesc();
		const unsigned char* GetRawData()override;
		UInt2 GetSize() const override {
			return UInt2(image.vlp_imageData->width, image.vlp_imageData->height);
		}
		std::int32_t GetFormat()const override;
		std::int32_t GetStride()const override;
		std::int32_t GetDataSize()const override;
	 protected:
		 Resource_Texture_Dx12() {
		 }
		void UpdateResourceRelease()override;
		void ResourceUpdate() override;
		D3D12_GPU_DESCRIPTOR_HANDLE handle;
		Image image;
		//テクスチャ用のデスクプリタ
		D3D12_RESOURCE_DESC textureResDesc;
		std::uint32_t texturePixelSize;
		//リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> texture;
		//GPUアップロードのためのリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resourceUploadHeap;
		//テクスチャファイルへのパス
		std::wstring fileName{ L"" };
		bool dataRefresh = true;

		Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;
	};

}