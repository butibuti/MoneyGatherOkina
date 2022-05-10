#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteAnimationComponent;

	class BeeSoulComponent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "BeeSoulComponent";
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
		void Move();
		void Animation();

		Value_weak_ptr<SpriteAnimationComponent> m_vwp_spriteAnimationComponent;

		float m_speed;

		std::int16_t m_life;

		std::int16_t m_animationFrame;
		std::int16_t m_animationRate;
		std::int8_t m_animationCount;
		std::int8_t m_maxAnimationCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(BeeSoulComponent, true);

