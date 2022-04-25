#pragma once
#include"stdafx.h"
#include"Resource_Mesh.h"
#include"ButiRendering_Dx12/Header/MeshPrimitive.h"
#include"../GameParts/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;
	class Resource_Mesh_Dx12:public Resource_Mesh
	{
	public:
		struct VertexBuffer {
			Microsoft::WRL::ComPtr<ID3D12Resource> bufferResource;
			Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUploadHeap;
			D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		};
		Resource_Mesh_Dx12(Value_ptr<GraphicDevice> arg_graphicDevice);
		void Draw(const std::uint32_t arg_vertexType)override;

		void Initialize()override;

		void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) override;

		void SetVertexBuffer(const std::uint32_t arg_vertexType, Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferResource, Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferUploadHeap, D3D12_VERTEX_BUFFER_VIEW arg_vertexBufferview);
		void SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indeexBuffer);
		void SetIndexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBufferUploadHeap);
		void SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW arg_indexBufferView);

		void SetVertexCount(const std::uint32_t arg_vertexCount);

		void SetIndexCount(const std::uint32_t arg_indexCount);


		const VertexBuffer& GetVertexBuffer(std::uint32_t arg_vertexType) { return map_vertexBuffer.at(arg_vertexType); }

		Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer();	


		Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBufferUploadHeap();

	private:
		std::map<std::uint32_t,VertexBuffer> map_vertexBuffer;	
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBufferUploadHeap;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
		

		Value_weak_ptr<GraphicDevice_Dx12> vwp_graphicDevice;


		void ResourceUpdate() override;

		void UpdateResourceRelease()override;

	};
	
}