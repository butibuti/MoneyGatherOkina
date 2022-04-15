#ifndef DESCRIPTORHEAPMANAGER_H
#define DESCRIPTORHEAPMANAGER_H


#pragma once
#include<memory>
#include <commctrl.h> 
#include <d3d12.h>
#include<wrl.h>
namespace ButiEngine {
class GraphicDevice_Dx12;
class BlankSpace;
struct HandleInformation {
	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle;
	std::uint32_t index;
};
struct BackUpConstantBufferData {
	std::uint32_t top;
	std::uint32_t sizeAligned;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
};
class DescriptorHeapManager
{
	const std::uint32_t DescriptorHeapSize = 8192;
public:
	DescriptorHeapManager(Value_weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, const std::uint32_t arg_max = 512);
	~DescriptorHeapManager();
	void Initialize(ID3D12Device& device);

	HandleInformation GetSamplerHandle(const std::uint32_t key);

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap();
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSamplerHeap();

	Microsoft::WRL::ComPtr<ID3D12Resource> GetConstantBuffer();

	void AddHeapRange();
	HandleInformation CreateConstantBufferView(void* arg_p_value, const bool arg_isKeep, const std::int32_t arg_size = 0x100);
	void ConstantBufferUpdate(void* arg_p_value, const std::uint32_t arg_index, const std::int32_t arg_size = 0x100);
	//HandleInformation CreateShaderResourceView(Microsoft::WRL::ComPtr<ID3D12Resource> resource ,const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc,const std::int32_t size );
	HandleInformation GetCurrentHandle(const std::int32_t arg_size = 0x100);
	HandleInformation GetCurrentHandle(std::uint32_t& arg_ref_top, const std::int32_t arg_size = 0x100);
	std::uint32_t GetDescriptorHandleIncrementSize();
	std::uint32_t GetIndex();
	void Release(const BlankSpace& arg_releaseSpace);
	void Release();
private:
	void ReCreateConstantBuffer();
	std::uint32_t cbvSrvDescriptorHandleIncrementSize = 0;
	std::uint32_t samplerDescriptorHandleIncrementSize = 0;
	std::uint32_t index = 0;
	std::uint32_t maxCbv=0;
	std::uint32_t ApplicationKeepRange = 10;
	std::uint32_t cbvSize=0;
	///デスクプリタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbvSrvUavDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> samplerDescriptorHeap;
	std::vector<BlankSpace> vec_space;
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBufferUploadHeap;
	struct buffer { char buf[0x100]; };
	
	buffer* mappedConstantBuffer = nullptr;
	std::vector<BackUpConstantBufferData*> vec_cbBackUpData;
	Value_weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
};

}


#endif // !DESCRIPTORHEAPMANAGER_H