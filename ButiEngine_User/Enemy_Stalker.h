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

		bool IsPrey() { return m_isPrey; }

		void Dead();
	private:
		void OnCollisionPlayer(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionWorker(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnCollisionEnemy(Value_weak_ptr<GameObject> arg_vwp_other);

		void Move();
		void Prey(Value_weak_ptr<GameObject> arg_vwp_other);
		void OnPrey();

		void SetEnemyParameter();

		Value_ptr<Enemy> m_vlp_enemy;

		Vector3 m_velocity;
		float m_maxMoveSpeed;
		float m_acceleration;

		Value_ptr<LookAtComponent> m_vlp_lookAt;
		Value_weak_ptr<GameObject> m_vwp_preyTarget;
		Value_ptr<Timer> m_vlp_preyTimer;
		bool m_isPrey;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Stalker, true);