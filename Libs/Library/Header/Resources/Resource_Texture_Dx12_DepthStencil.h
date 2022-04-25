#pragma once
#include<d3dx12.h>
#include"Resource_Texture_Dx12.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;


	class Resource_Texture_Dx12_DepthStencil :public Resource_Texture_Dx12, public IDepthStencil
	{
	public:
		Resource_Texture_Dx12_DepthStencil(Value_ptr<ImageFileIO::TextureResourceData> arg_vlp_imageData, Value_ptr<GraphicDevice> arg_vwp_graphicDevice);
		void CreateTextureUploadHeap()override;
		void Initialize()override;
		void UpdateResourceRelease()override;
		void ResourceUpdate() override;
		void Attach(std::int32_t slot)override;
		void SetIsCleared(bool arg_isClear)override;
		Vector2 GetSize()override;
		void SetDepthStencilView() override;
		void DisSetDepthStencil() override;
		std::int32_t GetFormat()const override;
		std::int32_t GetStride()const override;
		std::int32_t GetDataSize()const override;
		void OutputToMemory()override;
		const unsigned char* GetRawData()override;
	private:
		D3D12_RECT     scissorRect;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthStencilDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
		bool isCleared = false, isCopyCommanded = false;

#ifdef _EDITORBUILD
	public:
		void SetEditorViewed(const bool arg_isViewed)override { isEditorViewed = arg_isViewed; }
		bool GetEditorViewed()const override { return isEditorViewed; }
	private:
		bool isEditorViewed = false;
#endif
	};
}