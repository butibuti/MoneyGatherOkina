#pragma once
#include"stdafx.h"
#include"Resource_RealTimeMesh.h"

#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"../GameParts/GraphicDevice_Dx12.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;
	class Resource_RealTimeMesh_Dx12 :public Resource_RealTimeMesh
	{
	public:
		Resource_RealTimeMesh_Dx12(Value_ptr<GraphicDevice> arg_graphicDevice);
		void Draw(const std::uint32_t arg_vertexType)override;

		void Initialize()override;

		void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;

		void SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBuffer);
		void SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indeexBuffer);
		void SetVertexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferUploadHeap);
		void SetIndexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBufferUploadHeap);

		void SetVertexBufferView(D3D12_VERTEX_BUFFER_VIEW arg_vertexBufferview);
		void SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW arg_indexBufferView);

		void SetVertexCount(const std::uint32_t arg_vertexCount);

		void SetIndexCount(const std::uint32_t arg_indexCount);

		Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBuffer();

		Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer();

		Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBufferUploadHeap();

		Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBufferUploadHeap();

		void Update()override;

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUploadHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBufferUploadHeap;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;


		Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;


		void ResourceUpdate() override;

		void UpdateResourceRelease()override;

	};


}