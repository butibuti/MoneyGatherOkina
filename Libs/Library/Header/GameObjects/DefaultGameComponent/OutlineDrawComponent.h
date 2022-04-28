#pragma once
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

namespace ButiEngine {

	class OutlineMeshDrawComponent :public MeshDrawComponent
	{
	public:
		OutlineMeshDrawComponent();
		std::string GetGameComponentName() {
			return "OutlineMeshDrawComponent";
		}

		Value_ptr<GameComponent> Clone()override;
		void OnShowUI()override;
		void OnSet()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(meshTag);
			archive(shaderTag);
			archive(modelTag);
			archive(outlineMeshTag);
			archive(outlineShaderTag);
			archive(outlineModelTag);
			archive(materialTag);
			archive(isActive);
			archive(vlp_transform);
			archive(vlp_drawInfo);
			archive(vlp_outlineDrawInfo);
		}

		void CreateData()override;
	protected:
		MeshTag outlineMeshTag;
		ModelTag outlineModelTag;
		ShaderTag outlineShaderTag;

		Value_ptr<ButiRendering::DrawInformation >vlp_outlineDrawInfo = nullptr;

	}; 
	class OutlineModelDrawComponent :public ModelDrawComponent
	{
	public:
		OutlineModelDrawComponent();
		std::string GetGameComponentName() {
			return "OutlineModelDrawComponent";
		}

		Value_ptr<GameComponent> Clone()override;
		void OnShowUI()override;
		void OnSet()override;
		Value_ptr<ButiRendering::IModelObject> GetModelData()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(meshTag);
			archive(shaderTag);
			archive(modelTag);
			archive(outlineMeshTag);
			archive(outlineShaderTag);
			archive(outlineModelTag);
			archive(materialTag);
			archive(isActive);
			archive(vlp_transform);
			archive(vlp_drawInfo);
			archive(vlp_outlineDrawInfo);
		}

		void CreateData()override;
	protected:
		MeshTag outlineMeshTag;
		ModelTag outlineModelTag;
		ShaderTag outlineShaderTag;

		Value_ptr<ButiRendering::DrawInformation >vlp_outlineDrawInfo = nullptr;

	};
}


BUTI_REGIST_GAMECOMPONENT(OutlineModelDrawComponent,true)
BUTI_REGIST_GAMECOMPONENT(OutlineMeshDrawComponent,true)