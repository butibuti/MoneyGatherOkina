#pragma once
#include"stdafx.h"
#include"Vertex.h"
namespace ButiEngine {
	class GraphicDevice;
	class Resource_RealTimeMesh : public GPUResource, public IResource_Mesh
	{
	public:
		virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
		void Initialize()override {}
		void PreInitialize()override {}
		void SetBackupData(Value_ptr<ButiRendering::MeshPrimitiveBase> arg_vlp_meshPrimitive)override;
		std::uint32_t GetVertexCount();
		std::uint32_t GetIndexCount();
		virtual void Update()=0;
		const ButiRendering::MeshPrimitiveBase* GetBackUpdata(std::uint32_t arg_vertexType)const override;
		Value_ptr<ButiRendering::MeshPrimitiveBase> GetBackupData_Row();

		bool GetPosRayCast(Vector3* arg_p_pos,  Vector3* arg_p_normal, const Line& arg_line);
	protected:
		std::uint32_t verticesCount;
		std::uint32_t indexCount;
		std::uint32_t stride;
		bool canAccessWrite;
		bool isDataRefresh = true;
		std::uint32_t vertexType;
		Value_ptr<ButiRendering::MeshPrimitiveBase> vlp_meshPrimitive;


	};

}