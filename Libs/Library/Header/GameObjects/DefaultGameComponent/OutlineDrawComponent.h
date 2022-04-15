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
			archive(outlineMesh);
			archive(outlineShaderTag);
			archive(outlineModel);
			archive(materialTag);
			archive(isActive);
			archive(shp_transform);
			archive(shp_drawInfo);
			archive(shp_outlineDrawInfo);
		}

		void CreateData()override;
	protected:
		MeshTag outlineMesh;
		ModelTag outlineModel;
		ShaderTag outlineShaderTag;

		Value_ptr< DrawInformation >shp_outlineDrawInfo = nullptr;

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
		Value_ptr<ModelDrawData> GetModelData()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(meshTag);
			archive(shaderTag);
			archive(modelTag);
			archive(outlineMesh);
			archive(outlineShaderTag);
			archive(outlineModel);
			archive(materialTag);
			archive(isActive);
			archive(shp_transform);
			archive(shp_drawInfo);
			archive(shp_outlineDrawInfo);
		}

		void CreateData()override;
	protected:
		MeshTag outlineMesh;
		ModelTag outlineModel;
		ShaderTag outlineShaderTag;

		Value_ptr< DrawInformation >shp_outlineDrawInfo = nullptr;

	};
}


BUTI_REGIST_GAMECOMPONENT(OutlineModelDrawComponent,true)
BUTI_REGIST_GAMECOMPONENT(OutlineMeshDrawComponent,true)