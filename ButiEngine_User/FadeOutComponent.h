#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class FadeOutComponent : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "FadeOutComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

	private:
		float m_alpha;
	};
}

BUTI_REGIST_GAMECOMPONENT(FadeOutComponent, true);

