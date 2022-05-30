#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class VibrationEffectComponent;
	class ShakeComponent;
	class ParticleGenerater;
	class SpriteParticleGenerator;
	class Player;
	class SoundPlayerComponent;
	class EnemyScaleAnimationComponent;
	class WaveManager;
	class Flocking;

	class Worker :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Worker";
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

		Value_weak_ptr<GameObject> GetDrawObject();

		void Spawn();
		void SetIsVibrate(const bool arg_isVibrate) { m_isVibrate = arg_isVibrate; }

		void Dead();
		void Rupture(const Vector3& arg_dir);
		void Predated(Value_weak_ptr<GameObject> arg_vwp_other);
		void CreateAttackFlashEffect();
		void RemoveVibration(const float arg_vibration)
		{
			m_vibration -= arg_vibration; 
			m_vibration = max(m_vibration, m_minVibration);

			if (m_isTutorialVibrationObject)
			{
				m_vlp_vibrationResetTimer->Reset();
			}
		}
	private:
		void OnCollisionPlayer(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionStalker(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_enemy);
		void OnCollisionDamageArea(Value_weak_ptr<GameObject> arg_vwp_other);

		void OnNearPlayer();
		void OnRupture();

		void ReturnFlock();

		void StopVibrationEffect();
		void ShakeDrawObject();
		void StopShakeDrawObject();

		void IncreaseVibration();
		void DecreaseVibration();
		void ScaleAnimation();

		void CreateDrawObject();
		void SetLookAtParameter();
		void SetVibrationParameter();

		static float m_nearBorder;
		static float m_initVibrationForce;
		static float m_maxVibrationMagnification;
		static float m_maxVibration;
		static float m_minVibration;
		static float m_vibrationIncrease;
		static float m_vibrationDecrease;
		static float m_maxScaleRate;
		static float m_tutorialMaxVibration;
		static float m_spawnParticleSpeed;

		Value_weak_ptr<GameObject> m_vwp_tiltFloatObject;

		Value_weak_ptr<GameObject> m_vwp_vibrationEffect;
		Value_weak_ptr<GameObject> m_vwp_beeSoul;
		Value_weak_ptr<VibrationEffectComponent> m_vwp_vibrationEffectComponent;
		Value_weak_ptr<ShakeComponent> m_vwp_shakeComponent;
		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		Value_weak_ptr<SpriteParticleGenerator> m_vwp_spriteParticleGenerator;
		Value_weak_ptr<SpriteParticleGenerator> m_vwp_sphereParticleGenerator;
		Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;
		Value_weak_ptr<EnemyScaleAnimationComponent> m_vwp_scaleAnimationComponent;
		Value_ptr<LookAtComponent> m_vlp_lookAt;
		Value_ptr<Player> m_vlp_player;
		Value_weak_ptr<WaveManager> m_vwp_waveManager;
		Value_weak_ptr<Flocking> m_vwp_flocking;
		Value_ptr<Timer> m_vlp_nearPlayerTimer;
		Value_ptr<Timer> m_vlp_ruptureTimer;
		Value_ptr<Timer> m_vlp_attackFlashTimer;

		Vector3 m_defaultScale;

		bool m_isNearPlayer;
		bool m_isRupture;
		bool m_isVibrate;
		bool m_isAttack;
		bool m_isPredated;
		bool m_isAttackSE;
		
		float m_vibration;

		bool m_isTutorialVibrationObject;
		Value_ptr<Timer> m_vlp_vibrationResetTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(Worker, true);