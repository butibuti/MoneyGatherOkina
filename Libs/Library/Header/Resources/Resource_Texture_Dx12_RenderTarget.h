#pragma once
#include<d3dx12.h>
#include"Resource_Texture_Dx12.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;


	class Resource_Texture_Dx12_RenderTarget :public Resource_Texture_Dx12,public IRenderTarget
	{
	public:
		Resource_Texture_Dx12_RenderTarget(Value_ptr<ImageFileIO::TextureResourceData> arg_shp_imageData,const std::int32_t format, Value_ptr<GraphicDevice> arg_wkp_graphicDevice);
		void SetRenderTarget(Vector4& arg_clearColor)override;
		void CreateTextureUploadHeap()override;
		void Initialize()override;
		void DisSetRenderTarget()override;
		void  CopyForOutput()override;
		void UpdateResourceRelease()override;
		void ResourceUpdate() override;
		void Attach(std::int32_t slot)override; 
		void ToPNG(const std::string& arg_textureFilePath);
		GUIWindowReaction ShowTextureWindow(const std::string& arg_windowName ,const std::int32_t winFrag);
		void* GetResourcePtr()override;
		void SetIsCleared(bool arg_isClear)override;
		Vector2 GetSize()override;
		void OutputToMemory()override;
		std::int32_t GetFormat()const override;
		std::int32_t GetStride()const override;
		std::int32_t GetDataSize()const override;
		const unsigned char* GetRawData()override;
		void DataLock()override;
		void DataUnlock()override;
	private:

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> renderTargetDescriptorHeap;
		D3D12_RESOURCE_STATES currentState;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
		std::string outputFileName;
		//void UpdateResourceRelease()override;
		//void ResourceUpdate() override;
		bool isCleared = false,isCopyCommanded=false;
		bool isFileOutput = false;

#ifdef _EDITORBUILD
	public:
		void SetEditorViewed(const bool arg_isViewed)override;
		bool GetEditorViewed()const override { return isEditorViewed; }
		void Swap()override;
		void Wait()override;
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> resourceUploadHeap[2];
		void* p_outputRawData[2];
		bool isEditorViewed = false,currentWritingOutputBuffer=false, isRawDataChanged = false;
		std::mutex mtx_rawData;
		std::condition_variable cv_rawData[2];
		std::int32_t textureDataSize;
		
		std::lock_guard<std::mutex>* p_guard_rawData;
		D3D12_TEXTURE_COPY_LOCATION destLocation[2], srcLocation;
#else

		D3D12_TEXTURE_COPY_LOCATION destLocation, srcLocation;
#endif
	};
}