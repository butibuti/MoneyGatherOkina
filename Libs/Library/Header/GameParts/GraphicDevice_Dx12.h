#pragma once
#include"GraphicDevice.h"
#include <d3d12.h>
#include<dxgi1_6.h>

#include<mutex>
namespace ButiEngine {
class DescriptorHeapManager;
class PipelineStateManager;
enum FileFormat
{
	None,PNG,JPEG,BMP
};
class IApplication;
class GraphicDevice_Dx12 :public GraphicDevice
{
	struct ImageFileOutputInfo {
		GPUResource* p_outputResource;
		std::string outputName;
		FileFormat format;
	};
public:

	GraphicDevice_Dx12(Value_weak_ptr<IApplication> arg_wkp_application);
	void Initialize()override;

	void PreInitialize()override;

	void Release()override;

	void ClearDepthStancil(const float arg_depth=1.0f) override;

	ID3D12Device& GetDevice();

	HRESULT CreateCommittedResource(const D3D12_HEAP_PROPERTIES* pHeapProperties,D3D12_HEAP_FLAGS HeapFlags,const D3D12_RESOURCE_DESC* pDesc,D3D12_RESOURCE_STATES InitialResourceState,
		const D3D12_CLEAR_VALUE* pOptimizedClearValue,	const IID& riidResource, void** ppvResource);

	ID3D12CommandQueue& GetCommandQueue();

	ID3D12CommandAllocator& GetCommandAllocator();

	ID3D12CommandAllocator& GetCommandAllocator(const std::uint32_t arg_index);

	ID3D12CommandAllocator& GetBundleCommandAllocator();

	ID3D12GraphicsCommandList& GetCommandList();

	ID3D12GraphicsCommandList& GetUploadCommandList();
#ifdef _WINDOWGENERATE
	ID3D12GraphicsCommandList& GetClearCommandList();
#endif

	

	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateSrvSmpCbvMat(const std::uint32_t materialCount, const std::uint32_t srvCount, const std::uint32_t samplerCount, D3D12_ROOT_SIGNATURE_DESC& arg_rootSignatureDesc);

	void SetPipeLine(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);

	std::pair< Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC> GetRootSignature(const std::wstring& Key);

	Value_weak_ptr<DescriptorHeapManager> GetDescriptorHeapManager();

	PipelineStateManager& GetPipelineStateManager();

	void SetRootSignature(const std::wstring& Key, const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootsig, const D3D12_ROOT_SIGNATURE_DESC& arg_desc);

	void AddUploadResource(GPUResource* rarg_resource);

	void AddOutputResource(GPUResource* rarg_resource,const FileFormat arg_format,const std::string& arg_fileName);


	void UploadResourceRelease()override; 
	void UploadResourceBufferMerge()override;

	void UnSetCommandList();

	ID3D12Fence& GetFence();
#ifdef _WINDOWGENERATE
	IDXGISwapChain& GetSwapChain();
#endif // !_WINDOWGENERATE

	

	void WaitGPU();

	void Present() override;

	void DrawEnd();
	void StartGUICommand();
	void EndGUICommand();

	void ClearWindow() override;

	void SetCommandList(ID3D12GraphicsCommandList* arg_currentCommandList,const std::int32_t index=0);
	void SetDefaultRenderTarget()override;
	void SetDefaultDepthStencil();
	void CommandList_SetScissorRect();

	void CommandList_SetRenderTargetView();

	void PushRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& arg_rtvHandle);

	void InsertCommandList();

	void ResourceUpload()override;

	void DrawStart()override;
	void PipelineClear()override;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRtvHeap() const;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDsvHeap() const;

	ID3D12Resource& GetTextureUploadHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() const;

	D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() const;

	const D3D12_RECT& GetScissorRect()const;

	std::uint32_t GetFrameCount() {
		return FrameCount;
	}

	std::uint32_t GetFrameIndex() {
#ifdef _WINDOWGENERATE
		return frameIndex;
#else
		return 0;
#endif // !_WINDOWGENERATE
		
	}

	void SetDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE*  arg_dsv);
	D3D12_CPU_DESCRIPTOR_HANDLE* GetDepthStencil();
	void DisSetDepthStencil();
	void ResetPipeLine()override;
private:
	D3D12_CPU_DESCRIPTOR_HANDLE* currentDSV = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE defaultDSVHandle;
	static const std::uint32_t FrameCount =
#ifdef _WINDOWGENERATE

		3;
#else
		1;
#endif // !_WINDOWGENERATE

	Microsoft::WRL::ComPtr<ID3D12Device> device;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator[FrameCount];

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> bundleCommandAllocator;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> presentCommandList;
#ifdef _WINDOWGENERATE
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> clearCommandList;
#endif // !_WINDOWGENERATE

	

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> uploadCommandList;

	std::vector<GPUResource*> vec_uploadResources;
	std::vector<GPUResource*> vec_uploadResourcesBuffer;
	std::vector<GPUResource*> vec_befUploadResources;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> drawCommandList;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> guiCommandList;

	Value_ptr<DescriptorHeapManager> shp_descripterManager;

	Value_ptr<PipelineStateManager> shp_pipelineStateManager;

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> vec_renderTargetHandles;

	ID3D12GraphicsCommandList* p_currentCommandList=nullptr; 
	ID3D12PipelineState* currentPipelineState=nullptr;

	std::vector< ID3D12CommandList*> vec_drawCommandLists;
	Microsoft::WRL::ComPtr<ID3D12Fence>  fence;

	std::map<std::wstring, std::pair<Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC>> map_rootSignature;
	Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[FrameCount];

	Microsoft::WRL::ComPtr<ID3D12Resource> resourceUploadHeap;
	D3D12_RECT scissorRect;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencil;
	std::uint32_t depthDescriptorSize, renderTargetDescriptorSize;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>  renderTargetDescripterHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthStencilDescriptorHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE ary_rtvHandle[FrameCount];


	Microsoft::WRL::ComPtr<ID3D12RootSignature>  clearRootSignature;

#ifdef _WINDOWGENERATE
	Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain3;
	std::uint32_t frameIndex;
#else

	const std::uint32_t frameIndex=0;
#endif // !_WINDOWGENERATE

	HANDLE  fenceEvent;
	UINT64  fenceValue;

	std::vector<ImageFileOutputInfo > vec_outputInfo;
	std::mutex mtx_uploadResource,mtx_createCommitedResourece;
};
}