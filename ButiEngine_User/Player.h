#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ParticleGenerater;
	class WaveManager;
	class VibrationEffectComponent;
	class ShakeComponent;
	class NumberManagerComponent;
	class BeeSoulPodUIComponent;
	class FlockingLeader;
	class SoundPlayerComponent;
	class PauseManagerComponent;
	class SpriteParticleGenerator;

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
		float GetVibrationForce()
		{
			return m_initVibrationForce * m_maxVibrationMagnification * GetVibrationRate();
		}

		float GetVibration() { return m_vibration; }
		float GetVibrationRate()
		{
			float rate = m_vibration / m_maxVibration;
			rate = min(rate, 1.0f);
			return rate;
		}

		std::int8_t GetLife() { return m_life; }
		float GetNearEnemyVibrationRate() { return m_nearEnemyVibrationRate; }
		Vector3 GetVelocity() { return m_velocity; }

		void SetNearEnemyVibrationRate(const float arg_vibrationRate) { m_nearEnemyVibrationRate = arg_vibrationRate; }
		void SetVelocity(const Vector3& arg_velocity) { m_velocity = arg_velocity; }
		void SetStrongestNearWorkerVibration(const float arg_vibration)
		{
			m_strongestNearWorkerVibration = max(m_strongestNearWorkerVibration, arg_vibration);
		}

		bool IsDead() { return m_isDead; }
		bool IsBomb() { return m_isBomb; }
		bool IsOverheat() { return m_isOverheat; }

		void Dead();
		void Revival();
		void AddNearEnemyCount() { m_nearEnemyCount++; }
		void AddNearWorkerCount() { m_nearWorkerCount++; }
		void KnockBack(const Vector3& arg_velocity);
		void SetShockWaveScale(const Vector3& arg_scale);

	private:
		void Move();
		void MoveKnockBack();
		void Damage();
		void VibrationUpdate();

		void VibrationController();
		void IncreaseVibration();
		void DecreaseVibration();
		void VibrationEffect();
		void StopVibrationEffect();
		void VibrationPowerDrawUpdate();
		void ShakeDrawObject();
		void StartBomb();
		void Bomb();
		void StartOverheat();
		void Overheat();

		void OnInvincible();
		void OnCollisionDamageArea(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionStalker(Value_weak_ptr<GameObject> arg_vwp_other);

		void CreateDamageEffect(Value_weak_ptr<GameObject> arg_vwp_other);

		void CreateDrawObject();
		void CreateSensorObject();
		void CreateBombObject();
		void SetLookAtParameter();
		void SetVibrationParameter();

		void StopVibUpSE();

		//移動
		static float m_maxMoveSpeed;
		static float m_acceleration;
		static float m_deceleration;

		//無敵時間
		static std::int32_t m_invincibleFrame;

		//振動
		static float m_overheatMaxVibration;
		static std::int32_t m_overheatFrame;
		static float m_initVibrationForce;
		static float m_maxVibrationMagnification;
		static float m_vibrationIncrease;
		static float m_vibrationDecrease;

		Value_weak_ptr<WaveManager> m_vwp_waveManager;

		//パラメータ
		Vector3 m_defaultScale;
		std::int8_t m_life;
		std::uint8_t m_maxWorkerCount;

		//移動
		Value_ptr<LookAtComponent> m_vlp_lookAt;
		Value_ptr<ButiRendering::ICamera> m_vlp_camera;
		Vector3 m_prevPos;
		Vector3 m_velocity;
		float m_defaultMaxMoveSpeed;
		float m_bombMaxMoveSpeed;

		//ノックバック
		Vector3 m_knockBackVelocity;
		std::int8_t m_knockBackFrame;
		std::int8_t m_maxKnockBackFrame;
		bool m_isKnockBack;

		//無敵時間
		Value_ptr<Timer> m_vlp_invincibleTimer;
		bool m_isInvincible;

		bool m_isDead;

		//アニメーション
		Value_weak_ptr<ShakeComponent> m_vwp_shakeComponent;
		Value_weak_ptr<GameObject> m_vwp_tiltFloatObject;

		//振動
		Value_weak_ptr<GameObject> m_vwp_shockWave;
		Value_weak_ptr<GameObject> m_vwp_vibrationEffect;
		Value_weak_ptr<GameObject> m_vwp_numberManager;
		Value_weak_ptr<GameObject> m_vwp_maxUI;
		Value_weak_ptr<GameObject> m_vwp_hzUIParent;
		Value_weak_ptr<FlockingLeader> m_vwp_flockingLeader;
		Value_weak_ptr<VibrationEffectComponent> m_vwp_vibrationEffectComponent;
		Value_weak_ptr<NumberManagerComponent> m_vwp_numberManagerComponent;
		Value_ptr<Timer> m_overheatTimer;
		Vector3 m_defaultNumberUIScale;
		Vector3 m_defaultMaxUIScale;
		bool m_isVibrate;
		bool m_isVibUpSE;
		float m_previousVibrationPower;
		float m_vibration;
		float m_maxVibration;
		std::uint8_t m_nearEnemyCount;
		std::uint8_t m_nearWorkerCount;
		float m_strongestNearWorkerVibration;
		float m_nearEnemyVibrationRate;
		bool m_isOverheat;
		float m_controllerVibration;
		std::int32_t m_vibUpCount;
		bool m_isFixNumberUIScale;
		std::string m_gameObjectName;

		//魂
		Value_weak_ptr<BeeSoulPodUIComponent> m_vwp_beeSoulPod;

		//センサー
		Value_weak_ptr<GameObject> m_vwp_sensor;
		Vector3 m_minSensorScale;
		Vector3 m_maxSensorScale;

		//ボム
		Value_weak_ptr<GameObject> m_vwp_bomb;
		Value_ptr<Timer> m_vlp_bombTimer;
		bool m_isBomb;

		//パーティクル
		Value_ptr<Timer> m_vlp_particleTimer;
		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		Value_weak_ptr<ParticleGenerater> m_vwp_polygonParticleGenerater;
		Value_weak_ptr<SpriteParticleGenerator> m_vwp_spriteParticleGenerater;

		Value_weak_ptr<GameObject> m_vwp_vignetteUI;
		Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;
		Value_weak_ptr<PauseManagerComponent> m_vwp_pauseManagerComponent;
		Value_ptr<Timer> m_vlp_vibUpSEResetTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);