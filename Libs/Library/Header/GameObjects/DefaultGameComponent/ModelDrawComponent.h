#pragma once
#include"MeshDrawComponent.h"

namespace ButiEngine {

	class ModelAnimation;
	class IModelObject;
	class ModelDrawComponent :public MeshDrawComponent
	{
	public:
		ModelDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, Value_ptr< DrawInformation >arg_vlp_drawInfo,  Value_ptr<Transform> arg_vlp_transform = nullptr);
		ModelDrawComponent() {}
		std::string GetGameComponentName()const override {
			return "ModelDraw";
		}

		void OnSet()override;
		virtual Value_ptr<IModelObject> GetModelData();
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
			archive(vlp_transform);
			archive(vlp_drawInfo);
		}


	protected:
		Value_ptr<IBoneObject> vlp_bone;
		void CreateData()override;
		Value_ptr<IModelObject> vlp_modelData;
	private:
	};

}

BUTI_REGIST_GAMECOMPONENT(ModelDrawComponent,true)

CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::MeshDrawComponent, ButiEngine::ModelDrawComponent);