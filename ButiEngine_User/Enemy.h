#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class WaveManager;
	class VibrationEffectComponent;
	class ShakeComponent;
	class EnemyScaleAnimationComponent;
	class ParticleGenerater;
	class SpriteParticleGenerator;
	class SoundPlayerComponent;
	class PauseManagerComponent;

	class Enemy :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy";
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

		void SetVibrationCapacity(const float arg_capacity) { m_vibrationCapacity = arg_capacity; }
		void SetVibrationResistance(const float arg_resistance) { m_vibrationResistance = arg_resistance; }
		void SetIsNearPlayer(const bool arg_isNearPlayer) { m_isNearPlayer = arg_isNearPlayer; }
		void SetWeight(const float arg_weight) { m_weight = arg_weight; }
		void SetIsCapaOver(const bool arg_isCapaOver) { m_isCapaOver = arg_isCapaOver; }
		void SetVibration(const float arg_vibration) { m_vibration = arg_vibration; }
		void SetProgressPoint(const std::int32_t arg_progressPoint) { m_progressPoint = arg_progressPoint; }
		void SetKnockBackForce(const float arg_knockBackForce) { m_knockBackForce = arg_knockBackForce; }
		void SetKnockBackFrame(const std::int32_t arg_knockBackFrame) { m_knockBackFrame = arg_knockBackFrame; }
		void SetIsDead(const bool arg_isDead) { m_isDead = arg_isDead; }

		Value_weak_ptr<GameObject> GetPlayer() { return m_vwp_player; }
		Value_weak_ptr<GameObject> GetNearFreePocket(const Vector3& arg_pos, float arg_border);
		float GetVibrationRate() { return m_vibration / m_vibrationCapacity; }
		float GetWeight() { return m_weight; }
		std::int32_t GetProgressPoint() { return m_progressPoint; }

		std::vector<Value_weak_ptr<GameObject>> GetStickWorkers();

		bool IsVibrate();
		bool IsNearPlayer() { return m_isNearPlayer; }
		bool IsCapaOver() { return m_isCapaOver; }

		void Dead();
		void CreatePocket(const std::uint8_t arg_pocketCount, const float arg_radius);
		void RemovePocket(const std::uint8_t arg_pocketNum);
		void RemoveAllPocket();
		void AddStickWorkerCount() { m_stickWorkerCount++; }
		void RemoveSrickWorkerCount() { m_stickWorkerCount--; }
		void SetAppearanceEffect(Value_weak_ptr<GameObject>arg_vwp_appearnceEffect);
		Value_weak_ptr<GameObject>GconstetAppearanceEffect();
	private:
		void IncreaseVibration();
		void DecreaseVibration();
		void VibrationStickWoker();
		void ShakeDrawObject();
		void ScaleAnimation();
		void AttackFlashUpdate();
		void CreateAttackFlashEffect();
		void DeadSound(const bool arg_isBigSound);
		void CreateProgressPointUI();

		//?U???????????l???v?Z????
		void CalculateVibrationIncrease();
		std::uint8_t GetStickWorkerCount();
		void AddPoint();
		void StopVibrationEffect();
		void MobDamegeSE();
		void StopMobDamageSE();

		void RuptureStickWorker();
		float CalculateWorkerVibrationForce();

		void OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_other);

		//?U??????????
		static float m_vibrationDecrease;
		static float m_playerVibrationCoefficient;

		Value_weak_ptr<GameObject> m_vwp_player;
		Value_weak_ptr<WaveManager> m_vwp_waveManager;
		Value_weak_ptr<GameObject> m_vwp_vibrationEffect;
		Value_weak_ptr<VibrationEffectComponent> m_vwp_vibrationEffectComponent;
		Value_weak_ptr<ShakeComponent> m_vwp_shakeComponent;
		Value_weak_ptr<EnemyScaleAnimationComponent> m_vwp_scaleAnimationComponent;
		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		Value_weak_ptr<SpriteParticleGenerator> m_vwp_spriteParticleGenerater;
		Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;
		Value_weak_ptr<PauseManagerComponent> m_vwp_pauseManagerComponent;
		Value_ptr<Player> m_vlp_playerComponent;
		Value_ptr<Timer> m_vlp_attackFlashTimer;
		Value_weak_ptr<GameObject>m_vwp_appearnceEffect;
		std::vector<Value_weak_ptr<GameObject>> m_vec_pockets;

		bool m_isDead;

		bool m_isNearPlayer;
		bool m_isHitShockWave;
		std::int16_t m_stickWorkerCount;
		Vector3 m_defaultScale;

		float m_weight;

		std::int32_t m_progressPoint;

		float m_vibration;
		float m_vibrationIncrease;
		float m_vibrationCapacity;
		float m_vibrationResistance;
		bool m_isCapaOver;
		bool m_isMobDamageSE;

		std::string m_gameObjectName;

		float m_knockBackForce;
		std::int32_t m_knockBackFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy, true);