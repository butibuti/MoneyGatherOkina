#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class SpriteAnimationComponent;

	class WorkerAttackFlashEffect : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "WorkerAttackFlashEffect";
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

		void Dead();
	private:
		void Animation();

		Value_ptr<SpriteAnimationComponent> m_vlp_spriteAnimation;
		Value_ptr<Timer> m_vlp_animationIntervalTimer;
	};
}

BUTI_REGIST_GAMECOMPONENT(WorkerAttackFlashEffect, true);