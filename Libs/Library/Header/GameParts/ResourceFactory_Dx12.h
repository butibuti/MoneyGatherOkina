#pragma once
#include"stdafx.h"
#include"../Resources/Resource_Mesh_Dx12.h"	
#include"../Resources/Resource_RealTimeMesh_Dx12.h"
#include"ResourceFactory.h"
#include"../GameParts/GraphicDevice_Dx12.h"
namespace ButiEngine {
class ResourceFactory_Dx12 :public ResourceFactory {
public:
	ResourceFactory_Dx12(Value_weak_ptr<GraphicDevice> arg_wkp_graphicDevice);
	Value_ptr<IResource_Texture> CreateTextureFromFile(const std::string& filePath)override;
	Value_ptr<IResource_Texture> CreateTextureFromPixelData(Value_ptr<ImageFileIO::TextureResourceData> arg_data);
	Value_ptr<IResource_Texture> CreateTextureFromPixelData(void* arg_p_pixelSrc, const std::int32_t arg_width, const std::int32_t arg_height, const std::int32_t arg_pixelSize, const std::int32_t arg_format) override;
	Value_ptr<IResource_Texture> CreateRenderTargetTexture(const std::uint32_t width, const std::uint32_t height, const std::int32_t format)override;
	Value_ptr<IResource_Texture> CreateDepthStencilTexture(const std::uint32_t width, const std::uint32_t height)override;
	Value_ptr<IResource_Texture> CreateEmptyTexture(const std::uint32_t width, const std::uint32_t height)override;


	Value_ptr<IResource_PixelShader> CreatePixelShaderFromFile(const std::string& filePath)override;
	Value_ptr<IResource_VertexShader> CreateVertexShaderFromFile(const std::string& filePath)override;
	Value_ptr<IResource_GeometryShader> CreateGeometryShaderFromFile(const std::string& filePath)override;
	Value_ptr<IResource_Material> CreateMaterial(const MaterialValue& arg_materialVar, const TextureTag& arg_textureTag)override;
	Value_ptr<IResource_Material> CreateMaterial(const MaterialValue& arg_materialVar, const std::vector< TextureTag>& arg_textureTag)override;
	Value_ptr<IResource_Material> CreateMaterialList(const MaterialValue& arg_materialVar, const std::vector< TextureTag>& arg_textureTag, Value_ptr<IResourceContainer> arg_shp_resourceContainer)override;
	Value_ptr<IResource_Mesh> CreateMesh(const List< ButiEngine::Value_ptr< ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData) override;
	Value_ptr<Resource_RealTimeMesh> CreateRealTimeMesh(const List< ButiEngine::Value_ptr< ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData)override;


};



}

