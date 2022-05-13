#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class SceneChangeAnimationComponent : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "SceneChangeAnimationComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

	private:

	};
}

BUTI_REGIST_GAMECOMPONENT(SceneChangeAnimationComponent, true);

