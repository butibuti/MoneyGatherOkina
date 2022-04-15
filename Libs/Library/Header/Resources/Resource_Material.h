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
		Value_ptr<IResource_Texture>GetTexture(const std::int32_t index = 0);
		Value_ptr<IResource_Texture>GetSphereTexture();
		
		std::vector< TextureTag > GetTextureTags()override {
			return textureTag;
		}
		TextureTag GetSphereTextureTag() const{
			return sphereTextureTag;
		}
		TextureTag GetTextureTag(const std::int32_t index=0)const {
			return textureTag[index];
		}
		std::vector< TextureTag >textureTag;
		TextureTag sphereTextureTag ;
	protected:
		MaterialValue materialVar;
		std::string materialName;
		std::string materialNameEng;
		std::string comment;
	};

}