#pragma once
#include"ButiRendering_Dx12/Header/Vertex.h"
namespace ButiEngine {
	class GraphicDevice;
	class Resource_Mesh : public GPUResource,public IResource_Mesh
	{
	public:
		virtual void SetGraphicDevice(Value_ptr<GraphicDevice> arg_graphicDevice) = 0;
		void Initialize()override {}
		void PreInitialize()override {}
		void SetBackupData(Value_ptr<ButiRendering::MeshPrimitiveBase> arg_vlp_meshPrimitive)override;
		std::uint32_t GetVertexCount();
		std::uint32_t GetIndexCount();
		bool GetPosRayCast(Vector3* arg_p_pos, Vector3* arg_p_normal, const Line& arg_line);
		const ButiRendering::MeshPrimitiveBase* GetBackUpdata(std::uint32_t arg_vertexType)const override;
		const std::string& GetTagName()const override;
		void SetTagName(const std::string& arg_tagName);
	protected:
		std::uint32_t verticesCount;
		std::uint32_t indexCount;
		std::uint32_t stride;
		bool canAccessWrite;
		bool isDataRefresh = true;
		std::string tagName;
		std::map<std::uint32_t,Value_ptr<ButiRendering::MeshPrimitiveBase>> map_vlp_meshPrimitive;


	};

}