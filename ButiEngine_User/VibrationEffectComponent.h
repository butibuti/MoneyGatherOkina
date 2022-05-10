#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteAnimationComponent;

	class VibrationEffectComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "VibrationEffectComponent";
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

		void SetVibrationViolent(const float arg_vibrationPower, const bool arg_isPlayer);

	private:
		void Animation();

		Value_weak_ptr<SpriteAnimationComponent> m_vwp_spriteAnimationComponent;

		Vector3 m_defaultScale;
		Vector3 m_calcScale;

		std::int16_t m_animationFrame;
		std::int32_t m_animationRate;
		std::int16_t m_animationCount;
		std::int16_t m_maxAnimationCount;

	};

}

BUTI_REGIST_GAMECOMPONENT(VibrationEffectComponent, true);

