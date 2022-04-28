#pragma once
#include"stdafx.h"
#include"ResourceFactory.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_Mesh_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Resource_RealTimeMesh_Dx12.h"
namespace ButiEngine {
class ResourceFactory_Dx12 :public ResourceFactory {
public:
	ResourceFactory_Dx12(Value_weak_ptr<ButiRendering::GraphicDevice> arg_vwp_graphicDevice);
	Value_ptr<ButiRendering::IResource_Texture> CreateTextureFromFile(const std::string& filePath)override;
	Value_ptr<ButiRendering::IResource_Texture> CreateTextureFromPixelData(Value_ptr<ImageFileIO::TextureResourceData> arg_data);
	Value_ptr<ButiRendering::IResource_Texture> CreateTextureFromPixelData(void* arg_p_pixelSrc, const std::int32_t arg_width, const std::int32_t arg_height, const std::int32_t arg_pixelSize, const std::int32_t arg_format) override;
	Value_ptr<ButiRendering::IResource_Texture> CreateRenderTargetTexture(const std::uint32_t width, const std::uint32_t height, const std::int32_t format)override;
	Value_ptr<ButiRendering::IResource_Texture> CreateDepthStencilTexture(const std::uint32_t width, const std::uint32_t height)override;
	Value_ptr<ButiRendering::IResource_Texture> CreateEmptyTexture(const std::uint32_t width, const std::uint32_t height)override;


	Value_ptr<ButiRendering::IResource_PixelShader> CreatePixelShaderFromFile(const std::string& filePath)override;
	Value_ptr<ButiRendering::IResource_VertexShader> CreateVertexShaderFromFile(const std::string& filePath)override;
	Value_ptr<ButiRendering::IResource_GeometryShader> CreateGeometryShaderFromFile(const std::string& filePath)override;
	Value_ptr<ButiRendering::IResource_Material> CreateMaterial(const ButiRendering::MaterialValue& arg_materialVar, Value_weak_ptr<ButiRendering::IResource_Texture> arg_vwp_texture);
	Value_ptr<ButiRendering::IResource_Material> CreateMaterial(const ButiRendering::MaterialValue& arg_materialVar, const List< Value_weak_ptr<ButiRendering::IResource_Texture>>& arg_list_vwp_texture) override;
	Value_ptr<ButiRendering::IResource_Material> CreateMaterialList(const ButiRendering::MaterialValue& arg_materialVar, const List< Value_weak_ptr<ButiRendering::IResource_Texture>>& arg_list_vwp_texture, Value_ptr<IResourceContainer> arg_vlp_resourceContainer)override;
	Value_ptr<ButiRendering::IResource_Mesh> CreateMesh(const std::string& arg_meshName, const List< ButiEngine::Value_ptr< ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData) override;
	Value_ptr<ButiRendering::Resource_RealTimeMesh> CreateRealTimeMesh(const std::string& arg_meshName, const List< ButiEngine::Value_ptr< ButiRendering::MeshPrimitiveBase>>& arg_list_vlp_inputMeshData)override;


};



}

