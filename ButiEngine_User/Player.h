#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ParticleGenerater;
	class WaveManager;
	class VibrationEffectComponent;
	class ShakeComponent;

	class Player :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Player";
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

		std::uint8_t GetMaxWorkerCount() { return m_maxWorkerCount; }
		float GetMoveSpeed()
		{ 
			float speed = (gameObject.lock()->transform->GetLocalPosition() - m_prevPos).GetLength();
			speed = min(speed, m_maxMoveSpeed);
			return  speed;
		}
		float GetMaxMoveSpeed() { return m_maxMoveSpeed; }
		float GetVibrationForce() { return m_vibrationForce; }
		std::int8_t GetLife() { return m_life; }
		float GetNearEnemyVibrationRate() { return m_nearEnemyVibrationRate; }

		void SetIsIncrease(const bool arg_isIncrease) { m_isIncrease = arg_isIncrease; }
		void SetNearEnemyVibrationRate(const float arg_vibrationRate) { m_nearEnemyVibrationRate = arg_vibrationRate; }

		bool IsDead() { return m_isDead; }

		void Dead();
		void Revival();
		void AddExp();
		void AddNearEnemyCount() { m_nearEnemyCount++; }
		void KnockBack(const Vector3& arg_velocity);
		void SetShockWaveScale(const Vector3& arg_scale);
	private:
		void Move();
		void LevelUp();
		void MoveKnockBack();
		void TrajectoryParticleWaitCount();
		void Damage();

		void VibrationController();
		void IncreaseVibration();
		void DecreaseVibration();
		void VibrationEffect();
		void StopVibrationEffect();
		void ShakeDrawObject();

		void OnInvincible();
		void OnCollisionDamageArea(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionStalker(Value_weak_ptr<GameObject> arg_vwp_other);

		std::uint16_t CalculateRequestExp();
		void SetLookAtParameter();

		Value_weak_ptr<WaveManager> m_vwp_waveManager;

		//パラメータ
		Vector3 m_defaultScale;
		std::int8_t m_life;

		//レベル
		std::uint8_t m_level;
		std::uint8_t m_maxLevel;
		std::uint8_t m_maxWorkerCount;
		std::uint16_t m_exp;

		//移動
		Value_ptr<LookAtComponent> m_vlp_lookAt;
		Value_ptr<ButiRendering::ICamera> m_vlp_camera;
		Vector3 m_prevPos;
		Vector3 m_velocity;
		float m_maxMoveSpeed;
		float m_acceleration;
		float m_deceleration;

		//ノックバック
		Vector3 m_knockBackVelocity;
		std::int8_t m_knockBackFrame;
		std::int8_t m_maxKnockBackFrame;
		bool m_isKnockBack;

		//無敵時間
		Value_ptr<Timer> m_vlp_invincibleTimer;
		bool m_isInvincible;

		bool m_isDead;

		//振動
		Value_weak_ptr<GameObject> m_vwp_shockWave;
		Value_weak_ptr<GameObject> m_vwp_vibrationEffect;
		Value_weak_ptr<VibrationEffectComponent> m_vwp_vibrationEffectComponent;
		Value_weak_ptr<ShakeComponent> m_vwp_shakeComponent;
		float m_vibrationForce;
		bool m_isVibrate;
		bool m_isIncrease;
		float m_vibration;
		float m_maxVibration;
		std::uint8_t m_nearEnemyCount;
		float m_vibrationIncrease;
		float m_vibrationDecrease;
		float m_nearEnemyVibrationRate;

		//パーティクル
		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		std::int8_t m_addTrajectoryParticleCounter;
		std::int8_t m_addTrajectoryParticleWait;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);