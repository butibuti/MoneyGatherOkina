#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Enemy;
	class EnemyScaleAnimationComponent;
	class WarningMark;
	class SoundPlayerComponent;

	class Enemy_Volcano :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy_Volcano";
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
		void ShotVolcanoRock();
		void AddPredictedPoint();
		void ScaleAnimation();

		void SetEnemyParameter();

		bool IsDetectionPlayer();

		Value_ptr<Enemy> m_vlp_enemy;
		Value_ptr<Timer> m_vlp_rockShotTimer;
		Value_weak_ptr<EnemyScaleAnimationComponent> m_vwp_scaleAnimationComponent;
		Value_weak_ptr<WarningMark> m_vwp_warningMark;
		Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;

		static std::int32_t m_pocketCount;
		static float m_createPocketRadius;

		Vector3 m_randomVelocity;
		Vector3 m_defaultScale;
		Vector3 m_currentScale;
		Vector3 m_previousScale;

		std::int32_t m_rockShotRate;

		float m_detectionRange;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Volcano, true);

