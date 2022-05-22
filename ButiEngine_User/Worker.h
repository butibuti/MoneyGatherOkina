#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class VibrationEffectComponent;
	class ShakeComponent;
	class ParticleGenerater;
	class SpriteParticleGenerator;
	class Player;

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

		static float GetVibrationForce() { return m_vibrationForce; }

		void SetVibration(const bool arg_isVibration) { m_isVibrate = arg_isVibration; }

		void Dead();
		void Rupture(const Vector3& arg_dir);
		void Predated(Value_weak_ptr<GameObject> arg_vwp_other);
		void CreateAttackFlashEffect();
	private:
		void OnCollisionPlayer(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionStalker(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_enemy);
		void OnCollisionDamageArea(Value_weak_ptr<GameObject> arg_vwp_other);

		void OnNearPlayer();
		void OnRupture();

		void StopVibrationEffect();
		void ShakeDrawObject();
		void StopShakeDrawObject();

		void CreateDrawObject();
		void SetLookAtParameter();

		static float m_nearBorder;
		static float m_vibrationForce;

		Value_weak_ptr<GameObject> m_vwp_tiltFloatObject;

		Value_weak_ptr<GameObject> m_vwp_vibrationEffect;
		Value_weak_ptr<GameObject> m_vwp_beeSoul;
		Value_weak_ptr<VibrationEffectComponent> m_vwp_vibrationEffectComponent;
		Value_weak_ptr<ShakeComponent> m_vwp_shakeComponent;
		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		Value_weak_ptr<SpriteParticleGenerator> m_vwp_spriteParticleGenerater;
		Value_ptr<LookAtComponent> m_vlp_lookAt;
		Value_ptr<Player> m_vlp_player;
		Value_ptr<Timer> m_vlp_nearPlayerTimer;
		Value_ptr<Timer> m_vlp_ruptureTimer;
		Value_ptr<Timer> m_vlp_attackFlashTimer;

		Vector3 m_defaultScale;

		bool m_isNearPlayer;
		bool m_isRupture;
		bool m_isVibrate;
		bool m_isAttack;
		bool m_isPredated;
	};

}

BUTI_REGIST_GAMECOMPONENT(Worker, true);