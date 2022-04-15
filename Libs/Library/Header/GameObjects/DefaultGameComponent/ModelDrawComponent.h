#pragma once
#include"MeshDrawComponent.h"

namespace ButiEngine {

	class ModelAnimation;
	struct ModelDrawData;
	class ModelDrawComponent :public MeshDrawComponent
	{
	public:
		ModelDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, Value_ptr< DrawInformation >arg_shp_drawInfo,  Value_ptr<Transform> arg_shp_transform = nullptr);
		ModelDrawComponent() {}
		std::string GetGameComponentName() {
			return "ModelDraw";
		}

		void OnSet()override;
		virtual Value_ptr<ModelDrawData> GetModelData();
		Value_ptr<GameComponent> Clone()override;
		Value_ptr<IBoneObject> GetBone();
		void OnShowUI()override;


		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(meshTag);
			archive(shaderTag);
			archive(modelTag);
			archive(materialTag);
			archive(isActive);
			archive(shp_transform);
			archive(shp_drawInfo);
			//archive(shp_bone);
		}


	protected:
		Value_ptr<IBoneObject> shp_bone;
		void CreateData()override;
		Value_ptr<ModelDrawData> shp_modelData;
	private:
	};

}

BUTI_REGIST_GAMECOMPONENT(ModelDrawComponent,true)

CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::MeshDrawComponent, ButiEngine::ModelDrawComponent);