#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class FloatMotionComponent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FloatMotionComponent";
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
			archive(m_isRandomSpeed);
		}

		void SetAmplitude(const float arg_amplitude) { m_amplitude = arg_amplitude; }
		void SetMotionSpeed(const float arg_motionSpeed) { m_motionSpeed = arg_motionSpeed; }
		void SetIsRandomSpeed(const bool arg_isRandomSpeed) { m_isRandomSpeed = arg_isRandomSpeed; }
		void SetMinTargetSpeed(const float arg_minTargetSpeed) { m_minTargetSpeed = arg_minTargetSpeed; }
		void SetMaxTargetSpeed(const float arg_maxTargetSpeed) { m_maxTargetSpeed = arg_maxTargetSpeed; }
	private:
		void SetRandomSpeed();

		float m_theta;
		float m_amplitude;
		float m_motionSpeed;

		Value_ptr<Timer> m_vlp_changeSpeedInterval;
		bool m_isRandomSpeed = true;
		float m_targetSpeed;
		float m_minTargetSpeed;
		float m_maxTargetSpeed;
		float m_lerpSpeed;

		std::int32_t m_changeSpeedFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(FloatMotionComponent, true);

