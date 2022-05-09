#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteAnimationComponent;

	class SplashEffectComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "SplashEffectComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

	private:
		void Animation();

		Value_weak_ptr<SpriteAnimationComponent> m_vwp_spriteAnimationComponent;

		std::int16_t m_animationFrame;
		std::int16_t m_animationRate;
		std::int16_t m_animationCount;
		std::int16_t m_maxAnimationCount;

	};

}

BUTI_REGIST_GAMECOMPONENT(SplashEffectComponent, true);

