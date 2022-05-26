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
		void HaneAnimation(const bool arg_isDetectionPlayer);
		void FangAnimation(const bool arg_isDetectionPlayer);
		float CalculateRotationDirection();

		bool IsDetectionPlayer();

		static std::int32_t m_progressPoint;
		static std::int32_t m_pocketCount;
		static float m_createPocketRadius;
		static float m_vibrationCapacity;
		static float m_vibrationResistance;
		static float m_maxRotationAngle;
		static float m_rotationAcceleration;

		Value_ptr<Enemy> m_vlp_enemy;
		Value_ptr<Loiter> m_vlp_loiter;
		Value_weak_ptr<GameObject> m_vwp_fang;
		Value_ptr<LookAtComponent> m_vlp_lookAt;
		Value_weak_ptr<WarningMark> m_vwp_warningMark;

		//羽アニメーション
		Value_weak_ptr<MeshDrawComponent> m_vwp_hane_L;
		Value_weak_ptr<MeshDrawComponent> m_vwp_hane_R;
		float m_defaultHaneRotateY;
		float m_currentHaneRotateY_L;
		float m_currentHaneRotateY_R;

		//ファングアニメーション
		Value_ptr<Timer> m_fangAnimationTimer;
		Value_weak_ptr<MeshDrawComponent> m_vwp_fang_L;
		Value_weak_ptr<MeshDrawComponent> m_vwp_fang_R;
		float m_defaultFangRotateY;
		float m_currentFangRotateY_L;
		float m_currentFangRotateY_R;
		float m_endFangRotateY_L;
		float m_endFangRotateY_R;

		float m_detectionRange;

		//プレイヤーを見つけたときの回る処理
		float m_rotationAngle;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Kiba, true);