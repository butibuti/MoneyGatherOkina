#pragma once
#include "stdafx.h"
namespace ButiEngine {
	struct Resource_Material :public IResource_Material
	{
		Resource_Material(const MaterialValue& arg_var);
		void Initialize()override {};
		void PreInitialize()override {};
		const std::string& GetMaterialName();
		const std::string& GetMaterialNameEng();
		const std::string& GetMaterialComment();
		void SetMaterialName(const std::string& arg_name);
		void SetMaterialNameEng(const std::string& arg_name);
		void SetComment(const std::string& arg_comment);
		const std::string& GetTagName()const override;
		Value_weak_ptr<IResource_Texture>GetTexture(const std::int32_t index = 0)const;
		Value_weak_ptr<IResource_Texture>GetSphereTexture()const;
		void SetTexture(const std::uint32_t arg_index, Value_weak_ptr<IResource_Texture>  arg_vwp_texture)override;
		void SetSphereTexture(Value_weak_ptr<IResource_Texture>  arg_vwp_texture)override;
		const List<Value_weak_ptr<IResource_Texture>>& GetTextures()override {
			return list_vwp_texture;
		}
		List<Value_weak_ptr<IResource_Texture>> list_vwp_texture;
		Value_weak_ptr<IResource_Texture> vwp_texture ;
	protected:
		MaterialValue materialVar;
		std::string materialName;
		std::string materialNameEng;
		std::string comment;
	};

}