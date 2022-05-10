#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteAnimationComponent;

	class BeeSoulPodUIComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "BeeSoulPodUIComponent";
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

		void SetExpRate(const float arg_expRate);

	private:
		void Animation();

		Value_weak_ptr<SpriteAnimationComponent> m_vwp_spriteAnimationComponent;

		std::int8_t m_animationCount;
		std::int8_t m_maxAnimationCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(BeeSoulPodUIComponent, true);

