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
	class AttackFlashSpawner;

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
		void SetExplosionScale(const float arg_scale) { m_explosionScale = arg_scale; }
		void SetIsNearPlayer(const bool arg_isNearPlayer) { m_isNearPlayer = arg_isNearPlayer; }
		void SetWeight(const float arg_weight) { m_weight = arg_weight; }
		void SetIsCapaOver(const bool arg_isCapaOver) { m_isCapaOver = arg_isCapaOver; }
		void SetVibration(const float arg_vibration) { m_vibration = arg_vibration; }

		Value_weak_ptr<GameObject> GetPlayer() { return m_vwp_player; }
		Value_weak_ptr<GameObject> GetNearFreePocket(const Vector3& arg_pos, float arg_border);
		float GetVibrationRate() { return m_vibration / m_vibrationCapacity; }
		float GetWeight() { return m_weight; }

		std::vector<Value_weak_ptr<GameObject>> GetStickWorkers();

		bool IsVibrate();
		bool IsNearPlayer() { return m_isNearPlayer; }
		bool IsCapaOver() { return m_isCapaOver; }

		void Dead();
		void Explosion();
		void CreatePocket(const std::uint8_t arg_pocketCount, const float arg_radius);
		void RemovePocket(const std::uint8_t arg_pocketNum);
		void RemoveAllPocket();
		void AddStickWorkerCount() { m_stickWorkerCount++; }
		void RemoveSrickWorkerCount() { m_stickWorkerCount--; }
	private:
		void IncreaseVibration();
		void DecreaseVibration();
		void VibrationStickWoker();
		void ShakeDrawObject();
		void ScaleAnimation();
		void AttackFlashUpdate();
		void CreateAttackFlashEffect();
		bool pushP;
		//êUìÆó ÇÃè„è∏ílÇåvéZÇ∑ÇÈ
		void CalculateVibrationIncrease();
		std::uint8_t GetStickWorkerCount();
		void AddDeadCount();
		void StopVibrationEffect();

		void OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_other);

		static bool m_test_isExplosion;

		//êUìÆÇÃå∏è≠ó 
		static float m_vibrationDecrease;

		Value_weak_ptr<GameObject> m_vwp_player;
		Value_weak_ptr<WaveManager> m_vwp_waveManager;
		Value_weak_ptr<GameObject> m_vwp_vibrationEffect;
		Value_weak_ptr<VibrationEffectComponent> m_vwp_vibrationEffectComponent;
		Value_weak_ptr<ShakeComponent> m_vwp_shakeComponent;
		Value_weak_ptr<EnemyScaleAnimationComponent> m_vwp_scaleAnimationComponent;
		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		Value_weak_ptr<SpriteParticleGenerator> m_vwp_spriteParticleGenerater;
		Value_ptr<Player> m_vlp_playerComponent;
		Value_ptr<Timer> m_vlp_attackFlashTimer;

		std::vector<Value_weak_ptr<GameObject>> m_vec_pockets;

		bool m_isNearPlayer;
		bool m_isHitShockWave;
		std::uint8_t m_stickWorkerCount;
		Vector3 m_defaultScale;

		float m_weight;

		float m_vibration;
		float m_vibrationIncrease;
		float m_vibrationCapacity;
		float m_vibrationResistance;
		bool m_isCapaOver;

		static float m_playerVibrationCoefficient;

		float m_explosionScale;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy, true);