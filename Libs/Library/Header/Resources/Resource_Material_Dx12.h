#pragma once
#include"stdafx.h"
#include"Resource_Material.h"
#include"../Common/CBuffer_Dx12.h"
#include"../Common/CArrayBuffer_Dx12.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;
	class Resource_Material_Dx12:public Resource_Material
	{
	public:
		Resource_Material_Dx12(const MaterialValue& arg_var, const TextureTag& arg_textureTag, Value_weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice);
		Resource_Material_Dx12(const MaterialValue& arg_var, const std::vector< TextureTag>& arg_textureTag, Value_weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice);
		void Initialize()override ;
		void Attach(const std::uint32_t slotOffset, Value_ptr<IRenderer> arg_shp_renderer)override;
		void Update()override;
		std::uint32_t GetTextureCount()const override;
		bool OnShowUI() ;
		void SetTextureTag(const std::uint32_t index, TextureTag arg_tag) ;
		void SetMaterialVariable(const MaterialValue& arg_var);
		MaterialValue& GetMaterialVariable();
		MaterialValue_Deferred GetMaterialDeferredValue()override;
		void SetMaterialIndex(const std::int32_t arg_index)override;
	private:
		Value_weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
		Value_ptr< CBuffer_Dx12<MaterialValue>> materialBuffer;
	};

	class Resource_MaterialList_Dx12 :public Resource_Material {
	public:
		Resource_MaterialList_Dx12(const MaterialValue& arg_var, const std::vector< TextureTag>& arg_textureTag,Value_ptr<IResourceContainer> arg_resourceContainer,Value_weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice);
		void Initialize()override;
		void Attach(const std::uint32_t slotOffset, Value_ptr<IRenderer> arg_shp_renderer)override;
		void Update()override;
		std::uint32_t GetTextureCount()const override;
		bool OnShowUI();
		void SetTextureTag(const std::uint32_t index, TextureTag arg_tag);
		void SetMaterialVariable(const MaterialValue& arg_var);
		MaterialValue& GetMaterialVariable();
		MaterialValue_Deferred GetMaterialDeferredValue()override;
		void SetMaterialIndex(const std::int32_t arg_index)override;
	private:
		Value_weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
		Value_ptr< CArrayBuffer_Dx12<MaterialValue_Deferred>> materialBuffer;
		Value_weak_ptr<IResourceContainer> wkp_resourceContainer;
	};
}