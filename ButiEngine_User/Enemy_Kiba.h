#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Enemy;
	class Loiter;
	class WarningMark;

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

		void Dead();
	private:
		void LookAtPlayer();
		void CreateFang();
		void SetEnemyParameter();
		void SetLoiterParameter();
		void SetLookAtParameter();
		float CalculateRotationDirection();

		bool IsDetectionPlayer();

		Value_ptr<Enemy> m_vlp_enemy;
		Value_ptr<Loiter> m_vlp_loiter;
		Value_weak_ptr<GameObject> m_vwp_fang;
		Value_ptr<LookAtComponent> m_vlp_lookAt;
		Value_weak_ptr<WarningMark> m_vwp_warningMark;

		static std::int32_t m_pocketCount;
		static float m_createPocketRadius;

		float m_detectionRange;

		//プレイヤーを見つけたときの回る処理
		float m_rotationAngle;
		float m_maxRotationAngle;
		float m_rotationAcceleration;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Kiba, true);