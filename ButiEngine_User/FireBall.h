#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteParticleGenerator;

	enum class FireBallPhase
	{
		Wait,
		Appear,
		Rotate,
		Disappear
	};

	class FireBall :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FireBall";
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

		void SetIsStrengthened(const bool arg_isStrengthened) { m_isStrengthened = arg_isStrengthened; }
		void SetIsClockwise(const bool arg_isClockwise) { m_isClockwise = arg_isClockwise; }
		void SetDefaultScale(const Vector3& arg_defaultScale) { m_defaultScale = arg_defaultScale; }

		void DisappeaerStart();
		void Dead();
	private:
		void Wait();
		void WaitStart();
		void Appear();
		void AppearStart();
		void Rotate();
		void RotateStart();
		void Disappeaer();

		void KeepWorldScale();

		void SetPhaseParameter();
		void SetRotationParameter();

		Value_ptr<Timer> m_vlp_waitTimer;
		Value_ptr<Timer> m_vlp_appearTimer;
		Value_ptr<Timer> m_vlp_rotationTimer;
		Value_ptr<Timer> m_vlp_disappearTimer;

		FireBallPhase m_phase;

		bool m_isStrengthened;
		bool m_isClockwise;

		Vector3 m_defaultScale;
		Vector3 m_beforeDisappearScale;

		Value_ptr<Transform> m_vlp_rotationCenter;
		float m_rotationAngle;
		float m_maxRotationAngle;
		float m_rotationAcceleration;

		Value_weak_ptr<SpriteParticleGenerator> m_vwp_spriteParticleGenerator;
	};

}

BUTI_REGIST_GAMECOMPONENT(FireBall, true);