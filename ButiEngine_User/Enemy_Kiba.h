#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Enemy;
	class Loiter;

	class Enemy_Kiba :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy_Kiba";
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
	private:
		void LookAtPlayer();
		void CreateDamageArea();
		void SetEnemyParameter();
		void SetLoiterParameter();
		float CalculateRotationDirection();

		Value_ptr<Enemy> m_vlp_enemy;
		Value_ptr<Loiter> m_vlp_loiter;
		Value_weak_ptr<GameObject> m_vwp_damageArea;

		float m_rotationAngle;
		float m_maxRotationAngle;
		float m_rotationAcceleration;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Kiba, true);