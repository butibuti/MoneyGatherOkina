#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteAnimationComponent;

	class LoopAnimation :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "LoopAnimation";
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
			archive(m_animationInterval);
		}

		void SetAnimationInterval(const std::int32_t arg_animationInterval)
		{
			m_animationInterval = arg_animationInterval; 
			m_vlp_animationIntervalTimer->ChangeCountFrame(m_animationInterval);
		}

		void AnimationStart();
		void AnimationStop();
		void AnimationReset();
	private:
		Value_weak_ptr<SpriteAnimationComponent> m_vwp_spriteAnimation;
		Value_ptr<Timer> m_vlp_animationIntervalTimer;

		bool isAnimation;
		std::int32_t m_animationInterval;
	};

}

BUTI_REGIST_GAMECOMPONENT(LoopAnimation, true);