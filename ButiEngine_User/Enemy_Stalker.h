#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Enemy;

	class Enemy_Stalker :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy_Stalker";
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

		void ResetVelocity() { m_velocity = Vector3Const::Zero; }

		bool IsPrey() { return m_isPrey; }

		void Dead();
	private:
		void OnCollisionPlayer(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionWorker(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_other);

		void Move();
		void Prey(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnPrey();
		void PreyAnimation();

		void SetEnemyParameter();
		void SetLookAtParameter();

		static std::int32_t m_progressPoint;
		static std::int32_t m_pocketCount;
		static float m_createPocketRadius;
		static float m_vibrationCapacity;
		static float m_vibrationResistance;

		static float m_knockBackForce;
		static std::int32_t m_knockBackFrame;

		static float m_maxMoveSpeed;
		static float m_acceleration;

		Value_ptr<Enemy> m_vlp_enemy;

		Vector3 m_velocity;

		Value_ptr<LookAtComponent> m_vlp_lookAt;
		Value_weak_ptr<GameObject> m_vwp_preyTarget;
		Value_ptr<Timer> m_vlp_preyTimer;
		bool m_isPrey;

		float m_defaultLookSpeed;
		Vector3 m_lookTargetAnimationStartPos;
		Vector3 m_lookTargetMoveDir;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Stalker, true);