#pragma once
#include"MeshDrawComponent.h"

namespace ButiEngine {

	class ShadowComponent :public MeshDrawComponent
	{
	public:
		ShadowComponent() {}
		std::string GetGameComponentName() {
			return "ShadowComponent";
		}

		void OnSet()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
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
			archive(shp_meshDrawComponent);
		}
		void Regist()override;
		void UnRegist()override;
		void CreateData()override;
	protected:
		void RegistShadowDrop(Value_ptr<MeshDrawComponent> arg_shp_shadowDrop = nullptr);
		Value_ptr<MeshDrawComponent> shp_meshDrawComponent;

		Value_ptr<IBoneObject> shp_bone;
	};
}


BUTI_REGIST_GAMECOMPONENT(ShadowComponent,true)