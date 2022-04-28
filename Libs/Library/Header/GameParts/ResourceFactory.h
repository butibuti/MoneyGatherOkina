#pragma once
#include"stdafx.h"

namespace ButiEngine {

class ResourceFactory :public IObject {
public:
	virtual Value_ptr<ButiRendering::IResource_Texture> CreateTextureFromFile(const std::string& filePath) = 0;
	virtual Value_ptr<ButiRendering::IResource_Texture> CreateTextureFromPixelData(Value_ptr<ImageFileIO::TextureResourceData> arg_data) = 0;
	virtual Value_ptr<ButiRendering::IResource_Texture> CreateTextureFromPixelData(void* arg_p_pixelSrc, const std::int32_t arg_width, const std::int32_t arg_height, const std::int32_t arg_pixelSize, const std::int32_t arg_format) = 0;
	virtual	Value_ptr<ButiRendering::IResource_Texture> CreateRenderTargetTexture(const std::uint32_t width, const std::uint32_t height, const std::int32_t format)=0;
	virtual Value_ptr<ButiRendering::IResource_Texture> CreateDepthStencilTexture(const std::uint32_t width, const std::uint32_t height) = 0;
	virtual Value_ptr<ButiRendering::IResource_Texture> CreateEmptyTexture(const std::uint32_t width, const std::uint32_t height) = 0;
	Value_ptr<ButiRendering::IResource_Material> CreateMaterial(const std::string& arg_filePath, const std::string& arg_fileDirectory = "");
	virtual Value_ptr<ButiRendering::IResource_Material> CreateMaterial(const ButiRendering::MaterialValue& arg_materialVar, Value_weak_ptr<ButiRendering::IResource_Texture> arg_vwp_texture) = 0;
	virtual Value_ptr<ButiRendering::IResource_Material> CreateMaterial(const ButiRendering::MaterialValue& arg_materialVar, const List< Value_weak_ptr<ButiRendering::IResource_Texture>>& arg_list_vwp_texture) = 0;
	virtual Value_ptr<ButiRendering::IResource_Material> CreateMaterialList(const ButiRendering::MaterialValue& arg_materialVar, const List< Value_weak_ptr<ButiRendering::IResource_Texture>>& arg_list_vwp_texture, Value_ptr<IResourceContainer> arg_vlp_resourceContainer) = 0;
	virtual Value_ptr<ButiRendering::IResource_PixelShader> CreatePixelShaderFromFile(const std::string& filePath) = 0;
	virtual Value_ptr<ButiRendering::IResource_VertexShader> CreateVertexShaderFromFile(const std::string& filePath) = 0;
	virtual Value_ptr<ButiRendering::IResource_GeometryShader> CreateGeometryShaderFromFile(const std::string& filePath) = 0;
	virtual Value_ptr<ButiRendering::IResource_Mesh> CreateMesh(const std::string& arg_meshName, const List< ButiEngine::Value_ptr< ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData) = 0;
	virtual Value_ptr<ButiRendering::Resource_RealTimeMesh> CreateRealTimeMesh(const std::string& arg_meshName, const List< ButiEngine::Value_ptr< ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData) = 0;

	void Initialize()override {};
	void PreInitialize()override {};
	Value_ptr<ButiRendering::IResource_Model>CreateModel(const std::string& modelPath, const std::string& fileDirectory = "");
	Value_ptr<ButiRendering::IResource_Motion>CreateMotion(const std::string& modelPath, const std::string& fileDirectory = "");
protected:
	Value_weak_ptr<ButiRendering::GraphicDevice> vwp_graphicDevice;
private:
	void ReadIndex(std::vector<std::uint32_t>& indices, char arg_indexByteCount, BinaryReader& arg_reader);
	template<typename T>
	void ReadPosition(T& out, BinaryReader& arg_reader) {
		out.position = arg_reader.ReadVariable<Vector3>();
	}
	template<typename T>
	void inline ReadVertex(std::vector< T>& out, const std::int32_t arg_vertexCount, BinaryReader& arg_reader, ButiRendering::BoxSurface& arg_ref_output_boxSurface) {
		float up = 0, down = 0, left = 0, right = 0, front = 0, back = 0;
		out.reserve(arg_vertexCount);
		for (std::uint32_t i = 0; i < arg_vertexCount; i++) {
			auto vertex = arg_reader.ReadVariable<T>();
			if (up < vertex.position.y) {
				up = vertex.position.y;
			}
			else if (down > vertex.position.y) {
				down = vertex.position.y;
			}
			if (right < vertex.position.x) {
				right = vertex.position.x;
			}
			else if (left > vertex.position.x) {
				left = vertex.position.x;
			}
			if (front < vertex.position.z) {
				front = vertex.position.z;
			}
			else if (back > vertex.position.z) {
				back = vertex.position.z;
			}

			out.push_back(vertex);
		}
		arg_ref_output_boxSurface.up = up;
		arg_ref_output_boxSurface.down = down;
		arg_ref_output_boxSurface.left = left;
		arg_ref_output_boxSurface.right = right;
		arg_ref_output_boxSurface.back = back;
		arg_ref_output_boxSurface.front = front;
	}
	template<typename T>
	void ReadNormal(T& out, BinaryReader& arg_reader) {
		out.SetNormal(arg_reader.ReadVariable<Vector3>());

	}
	template<typename T>
	void ReadUV(T& out, BinaryReader& arg_reader) {
		out.SetUV(arg_reader.ReadVariable<Vector2>());
	}
	template<typename T, std::int32_t uvExCount>
	void ReadExUV(T& out,  BinaryReader& arg_reader) {
		if constexpr (uvExCount > 0) {

			out.SetUV_ex1(arg_reader.ReadVariable<Vector2>());
		}
		if constexpr (uvExCount > 1) {

			out.SetUV_ex2(arg_reader.ReadVariable<Vector2>());
		}
		if constexpr (uvExCount > 2) {
			out.SetUV_ex3(arg_reader.ReadVariable<Vector2>());
		}
		if constexpr (uvExCount > 3) {
			out.SetUV_ex4(arg_reader.ReadVariable<Vector2>());
		}
	}
};
}