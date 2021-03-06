#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteAnimationComponent;
	class PauseManagerComponent;

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

		void SetVibration();
		void SetVibrationViolent(const float arg_vibrationPower, const bool arg_isPlayer);
		void SetEffectPosition(const Vector3& arg_position);
		void SetDefaultScale(const Vector3& arg_scale) { m_defaultScale = arg_scale; }

	private:
		void Animation();

		Value_weak_ptr<SpriteAnimationComponent> m_vwp_spriteAnimationComponent;
		Value_weak_ptr<PauseManagerComponent> m_vwp_pauseManagerComponent;
		Value_ptr<Timer> m_vlp_animationTimer;

		Vector3 m_defaultScale;
		Vector3 m_calcScale;
		Vector3 m_currentScale;

		float m_randomPosY;

		std::int16_t m_animationCount;
		std::int16_t m_maxAnimationCount;

	};

}

BUTI_REGIST_GAMECOMPONENT(VibrationEffectComponent, true);

